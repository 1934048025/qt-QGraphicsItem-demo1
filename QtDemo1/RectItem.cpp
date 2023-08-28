#include "RectItem.h"

RectItem::RectItem()
{
	// ������������¼�
	this->setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable);
}

QRectF RectItem::boundingRect() const
{
	qreal adjust = 0.5;//Ϊ��һ��Ҫ�е���
	return QRectF(m_rect);
}

void RectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	
	//���
	while (itemList.size() > 0)
	{
		pItem = itemList[0];
		itemList.erase(itemList.begin());
		parentItem()->scene()->removeItem(pItem);
		delete pItem;
		pItem = nullptr;
	}
	//���±��
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

	m_handlesArea[Mouse_LeftTop] = QRectF(rect.left() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);                           // ����
	m_handlesArea[Mouse_RightTop] = QRectF(rect.right() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);         // ����
	m_handlesArea[Mouse_LeftBottom] = QRectF(rect.left() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);     // ����
	m_handlesArea[Mouse_RightBottom] = QRectF(rect.right() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);   // ����
	m_handlesArea[Mouse_MiddleTop] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);     // ����
	m_handlesArea[Mouse_MiddleBottom] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);
	m_handlesArea[Mouse_MiddleLeft] = QRectF(rect.left() - m_dHandleSize / 2, rect.center().y() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);    // ����
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
		case Mouse_LeftTop:         // ����
		case Mouse_RightBottom:     // ����
			this->setCursor(Qt::SizeFDiagCursor);
			break;
		case Mouse_LeftBottom:      // ����
		case Mouse_RightTop:        // ����
			this->setCursor(Qt::SizeBDiagCursor);
			break;
		case Mouse_MiddleTop:       // ��
		case Mouse_MiddleBottom:    // ��
			this->setCursor(Qt::SizeVerCursor);
			break;
		case Mouse_MiddleLeft:      // ��
		case Mouse_MiddleRight:     // ��
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
	rectMouseHandle = Mouse_None;       // ��
	this->setCursor(Qt::ArrowCursor);
	this->setSelected(false);//��ת���μ���Բ����Ҫ
	return QGraphicsItem::hoverLeaveEvent(event);
}

void RectItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	//�����ʽ
	setMouseCursor(event);
	//��ʾ����
	QPointF scenePos = event->scenePos();
	QPointF itemPos = this->parentItem()->mapFromScene(scenePos);
	QPoint point = itemPos.toPoint();
	QString str = "( " + QString::number(point.x())
		+ ", " + QString::number(point.y()) + ")";
	setToolTip(str);
	//qDebug() << str;//�����ӳ�
	return QGraphicsItem::hoverMoveEvent(event);
}

void RectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)  // �������
	{
		// ��ǰѡ��ͼ�α�Ե������ͼԪ�ߴ�ı䴦��
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
	if (event->buttons() & Qt::LeftButton)  // �������
	{
		// ��������һ�λ���ʱ���϶�
		if (m_bDrawFlag)
		{
			return;
		}

		// ��������϶�����ͼԪʱ����
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
	// ��ֹ����Ҽ������˵�������ƶ�
	if (event->buttons() & Qt::LeftButton)  // �������
	{
		// ֻ���������϶�
		if (rectMouseHandle != Mouse_None)
		{
			// �϶�������ָ�ѡ��
			//this->setSelected(true);
		}
	}

	return QGraphicsItem::mouseReleaseEvent(event);
}

void RectItem::ResizeRect(const QPointF& mousePos)
{
	// ��ǰ�޲�����ֱ�ӷ���
	if (rectMouseHandle == Mouse_None)
	{
		return;
	}

	// ׼���仯
	this->prepareGeometryChange();

	QRectF rect = m_rect;
	qreal dMouseX = mousePos.x();   // ��굱ǰ��x����
	qreal dMouseY = mousePos.y();   // ��굱ǰ��y����
	// ��ǰʣ��ĸ߶ȺͿ��
	m_pressRect = boundingRect();
	double dRemainWidth = m_pressRect.width();
	double dRemainHeight = m_pressRect.height();

	if (rectMouseHandle == Mouse_LeftTop || rectMouseHandle == Mouse_MiddleLeft || rectMouseHandle == Mouse_MiddleTop)    // ��������ϡ�����
	{   // ���½ǵĵ㱣�ֲ���

		// �����������ſ���ǲ����
		if (rectMouseHandle != Mouse_MiddleTop)
		{
			// ʣ����=�����ұ�x����-����ƶ���ǰx
			dRemainWidth = m_pressRect.right() - dMouseX;
			if (dRemainWidth < dMixSize)
			{
				dRemainWidth = dMixSize;
			}
		}

		// �����������Ÿ߶��ǲ����
		if (rectMouseHandle != Mouse_MiddleLeft)
		{
			// ʣ��߶�=���εײ�y-����ƶ���ǰy
			dRemainHeight = m_pressRect.bottom() - dMouseY;
			if (dRemainHeight < dMixSize)
			{
				dRemainHeight = dMixSize;
			}
		}

		// �����Ȼ��߸߶����ŵ���Сֵ��������������
		if (dRemainWidth > dMixSize && dRemainHeight > dMixSize)
		{
			// �������Ͻǵ�����
			rect.setTopLeft(QPointF(m_pressRect.right() - dRemainWidth, m_pressRect.bottom() - dRemainHeight));
		}
	}
	else if (rectMouseHandle == Mouse_LeftBottom || rectMouseHandle == Mouse_MiddleBottom)   // ��������¡���
	{   // �������Ͻǲ���

		// �����������ſ���ǲ����
		if (rectMouseHandle != Mouse_MiddleBottom)
		{
			// ʣ����=�����ұ�x����-����ƶ���ǰx
			dRemainWidth = m_pressRect.right() - dMouseX;
			if (dRemainWidth < dMixSize)
			{
				dRemainWidth = dMixSize;
			}
		}

		// ʣ��߶�=���εײ�y-����ƶ���ǰy
		dRemainHeight = dMouseY - m_pressRect.top();
		if (dRemainHeight < dMixSize)
		{
			dRemainHeight = dMixSize;
		}

		// �����Ȼ��߸߶����ŵ���Сֵ��������������
		if (dRemainWidth > dMixSize && dRemainHeight > dMixSize)
		{
			// �������½ǵ�����
			rect.setBottomLeft(QPointF(m_pressRect.right() - dRemainWidth, m_pressRect.top() + dRemainHeight));
		}
	}
	else if (rectMouseHandle == Mouse_RightBottom || rectMouseHandle == Mouse_MiddleRight)   // ��������¡���
	{   // �������Ͻ��䲻��
		// ʣ����=����ƶ���ǰx����-�������x����
		dRemainWidth = dMouseX - m_pressRect.left();
		if (dRemainWidth < dMixSize)
		{
			dRemainWidth = dMixSize;
		}

		// �����������Ÿ߶��ǲ����
		if (rectMouseHandle != Mouse_MiddleRight)
		{
			
			// ʣ��߶�=���εײ�y-����ƶ���ǰy
			dRemainHeight = dMouseY - m_pressRect.top();
			if (dRemainHeight < dMixSize)
			{
				dRemainHeight = dMixSize;
			}
		}

		// �����Ȼ��߸߶����ŵ���Сֵ��������������
		if (dRemainWidth > dMixSize && dRemainHeight > dMixSize)
		{
			// �������½ǵ�����
			rect.setBottomRight(QPointF(m_pressRect.left() + dRemainWidth, m_pressRect.top() + dRemainHeight));
		}
	}
	else if (rectMouseHandle == Mouse_RightTop)   // ����
	{   // �������½ǲ���

		// ʣ����=�����ұ�x����-����ƶ���ǰx
		dRemainWidth = dMouseX - m_pressRect.left();
		if (dRemainWidth < dMixSize)
		{
			dRemainWidth = dMixSize;
		}

		// ʣ��߶�=���εײ�y-����ƶ���ǰy
		dRemainHeight = m_pressRect.bottom() - dMouseY;
		if (dRemainHeight < dMixSize)
		{
			dRemainHeight = dMixSize;
		}

		// �����Ȼ��߸߶����ŵ���Сֵ��������������
		if (dRemainWidth >= dMixSize && dRemainHeight >= dMixSize)
		{
			// �������Ͻǵ�����
			rect.setTopRight(QPointF(m_pressRect.left() + dRemainWidth, m_pressRect.bottom() - dRemainHeight));
		}
	}


	this->setRect(rect);
}

