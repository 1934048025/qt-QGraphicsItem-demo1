#include "CircleItem.h"
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QString>

CircleItem::CircleItem()
{
	// 接收鼠标悬浮事件
	this->setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable);
}

CircleItem::CircleItem(QRectF r, bool pItemFlag)
{
	this->setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable);
	//宽高一致
	m_rect = r;
	this->pointItemFlag = pItemFlag;
}

CircleItem::~CircleItem()
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
	//
}

QRectF CircleItem::boundingRect() const
{
	qreal adjust = 0.5;//为何一定要有调整
	return QRectF(m_rect);
}

void CircleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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
	if (pointItemFlag)
	{
		UpdateHandlesArea();
		for (int i = 0; i < mouseNum - 1; i++) {
			pItem = new PointItem(m_handlesArea[i]);
			pItem->setParentItem(this);
			itemList.push_back(pItem);
			pItem = nullptr;
		}
	}
	//画圆
	painter->setPen(pen);
	painter->drawEllipse(m_rect);
	//画圆心的十字标
	painter->setPen(penMiddle);
	midPoint = getMiddlePoint();
	painter->drawLine(QLineF(midPoint.x() - middleLon / 2, midPoint.y(),
		midPoint.x() + middleLon / 2, midPoint.y()));//drawLine()接受的int型不准确，QLineF接受qreal
	painter->drawLine(QLineF(midPoint.x(), midPoint.y() - middleLon / 2,
		midPoint.x(), midPoint.y() + middleLon / 2));
}

QPoint CircleItem::getMiddlePoint()
{
	QPoint midPoint = QPoint(m_rect.left() + m_rect.width() / 2,
		m_rect.top() + m_rect.height() / 2);
	return midPoint;
}

void CircleItem::UpdateHandlesArea()
{
	QRectF rect = this->boundingRect();
	m_handlesArea[Mouse_MiddleTop] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);     // 中上
	m_handlesArea[Mouse_MiddleBottom] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);
	m_handlesArea[Mouse_MiddleLeft] = QRectF(rect.left() - m_dHandleSize / 2, rect.center().y() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);    // 中左
	m_handlesArea[Mouse_MiddleRight] = QRectF(rect.right() - m_dHandleSize / 2, rect.center().y() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);
}

CircleItem::mouseHandleCircle CircleItem::GetMouseHandle(QPointF pointF)
{
	UpdateHandlesArea();
	mouseHandleCircle mHandle;
	if (m_handlesArea[Mouse_MiddleTop].contains(pointF))
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

void CircleItem::setMouseCursor(QGraphicsSceneHoverEvent* event)
{
	//this->isSelected()
	if (true)
	{
		circleMouseHandle = GetMouseHandle(event->pos());
		switch (circleMouseHandle) {
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

void CircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{

	//setMouseCursor(event);
	return QGraphicsItem::hoverEnterEvent(event);
}

void CircleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	circleMouseHandle = Mouse_None;       // 无
	this->setCursor(Qt::ArrowCursor);

	return QGraphicsItem::hoverLeaveEvent(event);
}

void CircleItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	//鼠标样式
	setMouseCursor(event);
	//显示坐标,相对于以图片图形项的左上角为原点
	QPointF scenePos = event->scenePos();
	QPointF itemPos = this->parentItem()->mapFromScene(scenePos);
	QPoint point = itemPos.toPoint();
	QString str = "( " + QString::number(point.x())
		+ ", " + QString::number(point.y()) + ")";
	setToolTip(str);
	//qDebug() << str;//降低延迟
	return QGraphicsItem::hoverMoveEvent(event);
}

void CircleItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)  // 左键按下
	{
		// 当前选中图形边缘，进行图元尺寸改变处理
		if (circleMouseHandle != Mouse_None)
		{
			m_pressRect = boundingRect();
			return;
		}
	}

	return QGraphicsItem::mousePressEvent(event);
}

void CircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)  // 左键按下
	{
		// 处理鼠标第一次绘制时的拖动
		if (m_bDrawFlag)
		{
			return;
		}

		// 处理鼠标拖动缩放图元时绘制
		if (circleMouseHandle != Mouse_None)
		{
			ResizeRect(event->pos());
			return;
		}
	}
 	return QGraphicsItem::mouseMoveEvent(event);
}

void CircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	// 防止鼠标右键弹出菜单后，左击移动
	if (event->buttons() & Qt::LeftButton)  // 左键按下
	{
		// 只处理上下拖动
		if (circleMouseHandle != Mouse_None)
		{
			// 拖动结束后恢复选中
			//this->setSelected(true);
		}
	}
	
	return QGraphicsItem::mouseReleaseEvent(event);
	
}

void CircleItem::ResizeRect(const QPointF& mousePos)
{
	
	// 当前无操作，直接返回
	if (circleMouseHandle == Mouse_None)
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
	midPoint = getMiddlePoint();
	qreal dRemainWidth2 = m_pressRect.width() / 2;//宽的一半
	qreal dRemainHeight2 = m_pressRect.height() / 2;//长的一半
	

	if (circleMouseHandle == Mouse_MiddleLeft)//鼠标在左
	{
		dRemainWidth2 = midPoint.x() - dMouseX;
		if (dRemainWidth2 < dMixSize)
		{
			dRemainWidth2 = dMixSize;
		}
		dRemainHeight2 = dRemainWidth2;
	}else if (circleMouseHandle == Mouse_MiddleRight)//鼠标在右
	{
		dRemainWidth2 = dMouseX - midPoint.x();
		if (dRemainWidth2 < dMixSize)
		{
			dRemainWidth2 = dMixSize;
		}
		dRemainHeight2 = dRemainWidth2;
	}else if (circleMouseHandle == Mouse_MiddleTop)//鼠标在上
	{
		dRemainHeight2 = midPoint.y() - dMouseY;
		if (dRemainHeight2 < dMixSize)
		{
			dRemainHeight2 = dMixSize;
		}
		dRemainWidth2 = dRemainHeight2;
	}else if (circleMouseHandle == Mouse_MiddleBottom)//鼠标在下
	{
		dRemainHeight2 = dMouseY - midPoint.y();
		if (dRemainHeight2 < dMixSize)
		{
			dRemainHeight2 = dMixSize;
		}
		dRemainWidth2 = dRemainHeight2;
	}
	if (dRemainWidth2 > dMixSize && dRemainHeight2 > dMixSize)
	{
		// 计算圆位置
		rect.setRect(midPoint.x() - dRemainWidth2, midPoint.y() - dRemainHeight2,
			dRemainWidth2 * 2, dRemainHeight2 * 2);
	}
	this->setRect(rect);
	//改变大小，对圆环判断，父环还是子环，重新排
	//变大
	if (dynamic_cast<CircleItem*>(parentItem()) != nullptr) {
		if (boundingRect().contains(QPointF(parentItem()->boundingRect().left(),parentItem()->boundingRect().center().y())))
		{
			QGraphicsItem* g = parentItem()->parentItem();
			QGraphicsItem* p = parentItem();
			p->setParentItem(0);
			//交换相对坐标
			QPointF itemPos = this->pos();
			this->setPos(parentItem()->pos());
			p->setPos(itemPos);
			setParentItem(g);
			p->setParentItem(this);
			p->setFlag(QGraphicsItem::ItemIsMovable, false);
			this->setFlag(QGraphicsItem::ItemIsMovable);
		}
	}
	//变小
	QGraphicsItem* child = this->childItems().at(0);
	if (child != nullptr && child->boundingRect().contains(QPointF(boundingRect().left(),
						boundingRect().center().y()))) {
		QGraphicsItem* g = parentItem();
		this->setParentItem(0);
		QPointF itemPos = this->pos();
		this->setPos(child->pos());
		child->setPos(itemPos);
		child->setParentItem(g);
		this->setParentItem(child);
		this->setFlag(QGraphicsItem::ItemIsMovable, false);
		child->setFlag(QGraphicsItem::ItemIsMovable);
	}
}

qreal CircleItem::radius() const
{
	return m_rect.width() / 2;
}

