#include "RectItem.h"

RectItem::RectItem()
{
	// 接收鼠标悬浮事件
	this->setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable);
}

QRectF RectItem::boundingRect() const
{
	qreal adjust = 0.5;//为何一定要有调整
	return QRectF(m_rect);
}

void RectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	
	//清除
	while (itemList.size() > 0)
	{
		pItem = itemList[0];
		itemList.erase(itemList.begin());
		parentItem()->scene()->removeItem(pItem);
		delete pItem;
		pItem = nullptr;
	}
	//更新标点
	UpdateHandlesArea();
	for (int i = 0; i < mouseNum - 2; i++) {
		pItem = new PointItem(m_handlesArea[i]);
		pItem->setParentItem(this);
		itemList.push_back(pItem);
		pItem = nullptr;
	}
	painter->setPen(pen);
	painter->drawRect(m_rect);
}


void RectItem::UpdateHandlesArea()
{
	QRectF rect = m_rect;

	m_handlesArea[Mouse_LeftTop] = QRectF(rect.left() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);                           // 左上
	m_handlesArea[Mouse_RightTop] = QRectF(rect.right() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);         // 右上
	m_handlesArea[Mouse_LeftBottom] = QRectF(rect.left() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);     // 左下
	m_handlesArea[Mouse_RightBottom] = QRectF(rect.right() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);   // 右下
	m_handlesArea[Mouse_MiddleTop] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);     // 中上
	m_handlesArea[Mouse_MiddleBottom] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);
	m_handlesArea[Mouse_MiddleLeft] = QRectF(rect.left() - m_dHandleSize / 2, rect.center().y() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);    // 中左
	m_handlesArea[Mouse_MiddleRight] = QRectF(rect.right() - m_dHandleSize / 2, rect.center().y() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);
}

RectItem::mouseHandle RectItem::GetMouseHandle(QPointF pointF)
{
	UpdateHandlesArea();
	mouseHandle mHandle;
	if (m_handlesArea[Mouse_LeftTop].contains(pointF))
		mHandle = Mouse_LeftTop;
	else if (m_handlesArea[Mouse_RightTop].contains(pointF))
		mHandle = Mouse_RightTop;
	else if (m_handlesArea[Mouse_LeftBottom].contains(pointF))
		mHandle = Mouse_LeftBottom;
	else if (m_handlesArea[Mouse_RightBottom].contains(pointF))
		mHandle = Mouse_RightBottom;
	else if (m_handlesArea[Mouse_MiddleTop].contains(pointF))
		mHandle = Mouse_MiddleTop;
	else if (m_handlesArea[Mouse_MiddleBottom].contains(pointF))
		mHandle = Mouse_MiddleBottom;
	else if (m_handlesArea[Mouse_MiddleLeft].contains(pointF))
		mHandle = Mouse_MiddleLeft;
	else if (m_handlesArea[Mouse_MiddleRight].contains(pointF))
		mHandle = Mouse_MiddleRight;
	else
		mHandle = Mouse_None;
	return mHandle;
}

void RectItem::setMouseCursor(QGraphicsSceneHoverEvent* event)
{
	//this->isSelected()
	if (true)
	{
		rectMouseHandle = GetMouseHandle(event->pos());
		switch (rectMouseHandle) {
		case Mouse_LeftTop:         // 左上
		case Mouse_RightBottom:     // 右下
			this->setCursor(Qt::SizeFDiagCursor);
			break;
		case Mouse_LeftBottom:      // 左下
		case Mouse_RightTop:        // 右上
			this->setCursor(Qt::SizeBDiagCursor);
			break;
		case Mouse_MiddleTop:       // 上
		case Mouse_MiddleBottom:    // 下
			this->setCursor(Qt::SizeVerCursor);
			break;
		case Mouse_MiddleLeft:      // 左
		case Mouse_MiddleRight:     // 右
			this->setCursor(Qt::SizeHorCursor);
			break;
		default:
			this->setCursor(Qt::SizeAllCursor);
			break;
		}
	}
}
void RectItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	
	//setMouseCursor(event);
	return QGraphicsItem::hoverEnterEvent(event);
}

void RectItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	rectMouseHandle = Mouse_None;       // 无
	this->setCursor(Qt::ArrowCursor);
	this->setSelected(false);//旋转矩形及椭圆的需要
	return QGraphicsItem::hoverLeaveEvent(event);
}

void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	//鼠标样式
	setMouseCursor(event);
	//显示坐标
	QPointF scenePos = event->scenePos();
	QPointF itemPos = this->parentItem()->mapFromScene(scenePos);
	QPoint point = itemPos.toPoint();
	QString str = "( " + QString::number(point.x())
		+ ", " + QString::number(point.y()) + ")";
	setToolTip(str);
	//qDebug() << str;//降低延迟
	return QGraphicsItem::hoverMoveEvent(event);
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)  // 左键按下
	{
		// 当前选中图形边缘，进行图元尺寸改变处理
		if (rectMouseHandle != Mouse_None)
		{
			m_pressRect = boundingRect();
			return;
		}
	}

	return QGraphicsItem::mousePressEvent(event);
}

void RectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)  // 左键按下
	{
		// 处理鼠标第一次绘制时的拖动
		if (m_bDrawFlag)
		{
			return;
		}

		// 处理鼠标拖动缩放图元时绘制
		if (rectMouseHandle != Mouse_None)
		{
			ResizeRect(event->pos());
			return;
		}
	}
	return QGraphicsItem::mouseMoveEvent(event);
}

void RectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	// 防止鼠标右键弹出菜单后，左击移动
	if (event->buttons() & Qt::LeftButton)  // 左键按下
	{
		// 只处理上下拖动
		if (rectMouseHandle != Mouse_None)
		{
			// 拖动结束后恢复选中
			//this->setSelected(true);
		}
	}

	return QGraphicsItem::mouseReleaseEvent(event);
}

void RectItem::ResizeRect(const QPointF& mousePos)
{
	// 当前无操作，直接返回
	if (rectMouseHandle == Mouse_None)
	{
		return;
	}

	// 准备变化
	this->prepareGeometryChange();

	QRectF rect = m_rect;
	qreal dMouseX = mousePos.x();   // 鼠标当前的x坐标
	qreal dMouseY = mousePos.y();   // 鼠标当前的y坐标
	// 当前剩余的高度和宽度
	m_pressRect = boundingRect();
	double dRemainWidth = m_pressRect.width();
	double dRemainHeight = m_pressRect.height();

	if (rectMouseHandle == Mouse_LeftTop || rectMouseHandle == Mouse_MiddleLeft || rectMouseHandle == Mouse_MiddleTop)    // 鼠标在左上、左、上
	{   // 右下角的点保持不变

		// 向上拉伸缩放宽度是不变的
		if (rectMouseHandle != Mouse_MiddleTop)
		{
			// 剩余宽度=矩形右边x坐标-鼠标移动当前x
			dRemainWidth = m_pressRect.right() - dMouseX;
			if (dRemainWidth < dMixSize)
			{
				dRemainWidth = dMixSize;
			}
		}

		// 向左拉伸缩放高度是不变的
		if (rectMouseHandle != Mouse_MiddleLeft)
		{
			// 剩余高度=矩形底部y-鼠标移动当前y
			dRemainHeight = m_pressRect.bottom() - dMouseY;
			if (dRemainHeight < dMixSize)
			{
				dRemainHeight = dMixSize;
			}
		}

		// 如果宽度或者高度缩放到最小值，则不让再缩放了
		if (dRemainWidth > dMixSize && dRemainHeight > dMixSize)
		{
			// 计算左上角的坐标
			rect.setTopLeft(QPointF(m_pressRect.right() - dRemainWidth, m_pressRect.bottom() - dRemainHeight));
		}
	}
	else if (rectMouseHandle == Mouse_LeftBottom || rectMouseHandle == Mouse_MiddleBottom)   // 鼠标在左下、下
	{   // 保持右上角不变

		// 向下拉伸缩放宽度是不变的
		if (rectMouseHandle != Mouse_MiddleBottom)
		{
			// 剩余宽度=矩形右边x坐标-鼠标移动当前x
			dRemainWidth = m_pressRect.right() - dMouseX;
			if (dRemainWidth < dMixSize)
			{
				dRemainWidth = dMixSize;
			}
		}

		// 剩余高度=矩形底部y-鼠标移动当前y
		dRemainHeight = dMouseY - m_pressRect.top();
		if (dRemainHeight < dMixSize)
		{
			dRemainHeight = dMixSize;
		}

		// 如果宽度或者高度缩放到最小值，则不让再缩放了
		if (dRemainWidth > dMixSize && dRemainHeight > dMixSize)
		{
			// 计算左下角的坐标
			rect.setBottomLeft(QPointF(m_pressRect.right() - dRemainWidth, m_pressRect.top() + dRemainHeight));
		}
	}
	else if (rectMouseHandle == Mouse_RightBottom || rectMouseHandle == Mouse_MiddleRight)   // 鼠标在右下、右
	{   // 保持左上角落不变
		// 剩余宽度=鼠标移动当前x坐标-矩形左边x坐标
		dRemainWidth = dMouseX - m_pressRect.left();
		if (dRemainWidth < dMixSize)
		{
			dRemainWidth = dMixSize;
		}

		// 向右拉伸缩放高度是不变的
		if (rectMouseHandle != Mouse_MiddleRight)
		{
			
			// 剩余高度=矩形底部y-鼠标移动当前y
			dRemainHeight = dMouseY - m_pressRect.top();
			if (dRemainHeight < dMixSize)
			{
				dRemainHeight = dMixSize;
			}
		}

		// 如果宽度或者高度缩放到最小值，则不让再缩放了
		if (dRemainWidth > dMixSize && dRemainHeight > dMixSize)
		{
			// 计算右下角的坐标
			rect.setBottomRight(QPointF(m_pressRect.left() + dRemainWidth, m_pressRect.top() + dRemainHeight));
		}
	}
	else if (rectMouseHandle == Mouse_RightTop)   // 右上
	{   // 保持左下角不变

		// 剩余宽度=矩形右边x坐标-鼠标移动当前x
		dRemainWidth = dMouseX - m_pressRect.left();
		if (dRemainWidth < dMixSize)
		{
			dRemainWidth = dMixSize;
		}

		// 剩余高度=矩形底部y-鼠标移动当前y
		dRemainHeight = m_pressRect.bottom() - dMouseY;
		if (dRemainHeight < dMixSize)
		{
			dRemainHeight = dMixSize;
		}

		// 如果宽度或者高度缩放到最小值，则不让再缩放了
		if (dRemainWidth >= dMixSize && dRemainHeight >= dMixSize)
		{
			// 计算右上角的坐标
			rect.setTopRight(QPointF(m_pressRect.left() + dRemainWidth, m_pressRect.bottom() - dRemainHeight));
		}
	}


	this->setRect(rect);
}

