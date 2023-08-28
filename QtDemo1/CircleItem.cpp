#include "CircleItem.h"
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QString>

CircleItem::CircleItem()
{
	// ������������¼�
	this->setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable);
}

CircleItem::CircleItem(QRectF r, bool pItemFlag)
{
	this->setAcceptHoverEvents(true);
	setFlags(QGraphicsItem::ItemIsMovable);
	//���һ��
	m_rect = r;
	this->pointItemFlag = pItemFlag;
}

CircleItem::~CircleItem()
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
	//
}

QRectF CircleItem::boundingRect() const
{
	qreal adjust = 0.5;//Ϊ��һ��Ҫ�е���
	return QRectF(m_rect);
}

void CircleItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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
	//��Բ
	painter->setPen(pen);
	painter->drawEllipse(m_rect);
	//��Բ�ĵ�ʮ�ֱ�
	painter->setPen(penMiddle);
	midPoint = getMiddlePoint();
	painter->drawLine(QLineF(midPoint.x() - middleLon / 2, midPoint.y(),
		midPoint.x() + middleLon / 2, midPoint.y()));//drawLine()���ܵ�int�Ͳ�׼ȷ��QLineF����qreal
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
	m_handlesArea[Mouse_MiddleTop] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.top() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);     // ����
	m_handlesArea[Mouse_MiddleBottom] = QRectF(rect.center().x() - m_dHandleSize / 2, rect.bottom() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);
	m_handlesArea[Mouse_MiddleLeft] = QRectF(rect.left() - m_dHandleSize / 2, rect.center().y() - m_dHandleSize / 2, m_dHandleSize, m_dHandleSize);    // ����
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

void CircleItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{

	//setMouseCursor(event);
	return QGraphicsItem::hoverEnterEvent(event);
}

void CircleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	circleMouseHandle = Mouse_None;       // ��
	this->setCursor(Qt::ArrowCursor);

	return QGraphicsItem::hoverLeaveEvent(event);
}

void CircleItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	//�����ʽ
	setMouseCursor(event);
	//��ʾ����,�������ͼƬͼ��������Ͻ�Ϊԭ��
	QPointF scenePos = event->scenePos();
	QPointF itemPos = this->parentItem()->mapFromScene(scenePos);
	QPoint point = itemPos.toPoint();
	QString str = "( " + QString::number(point.x())
		+ ", " + QString::number(point.y()) + ")";
	setToolTip(str);
	//qDebug() << str;//�����ӳ�
	return QGraphicsItem::hoverMoveEvent(event);
}

void CircleItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)  // �������
	{
		// ��ǰѡ��ͼ�α�Ե������ͼԪ�ߴ�ı䴦��
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
	if (event->buttons() & Qt::LeftButton)  // �������
	{
		// ��������һ�λ���ʱ���϶�
		if (m_bDrawFlag)
		{
			return;
		}

		// ��������϶�����ͼԪʱ����
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
	// ��ֹ����Ҽ������˵�������ƶ�
	if (event->buttons() & Qt::LeftButton)  // �������
	{
		// ֻ���������϶�
		if (circleMouseHandle != Mouse_None)
		{
			// �϶�������ָ�ѡ��
			//this->setSelected(true);
		}
	}
	
	return QGraphicsItem::mouseReleaseEvent(event);
	
}

void CircleItem::ResizeRect(const QPointF& mousePos)
{
	
	// ��ǰ�޲�����ֱ�ӷ���
	if (circleMouseHandle == Mouse_None)
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
	midPoint = getMiddlePoint();
	qreal dRemainWidth2 = m_pressRect.width() / 2;//���һ��
	qreal dRemainHeight2 = m_pressRect.height() / 2;//����һ��
	

	if (circleMouseHandle == Mouse_MiddleLeft)//�������
	{
		dRemainWidth2 = midPoint.x() - dMouseX;
		if (dRemainWidth2 < dMixSize)
		{
			dRemainWidth2 = dMixSize;
		}
		dRemainHeight2 = dRemainWidth2;
	}else if (circleMouseHandle == Mouse_MiddleRight)//�������
	{
		dRemainWidth2 = dMouseX - midPoint.x();
		if (dRemainWidth2 < dMixSize)
		{
			dRemainWidth2 = dMixSize;
		}
		dRemainHeight2 = dRemainWidth2;
	}else if (circleMouseHandle == Mouse_MiddleTop)//�������
	{
		dRemainHeight2 = midPoint.y() - dMouseY;
		if (dRemainHeight2 < dMixSize)
		{
			dRemainHeight2 = dMixSize;
		}
		dRemainWidth2 = dRemainHeight2;
	}else if (circleMouseHandle == Mouse_MiddleBottom)//�������
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
		// ����Բλ��
		rect.setRect(midPoint.x() - dRemainWidth2, midPoint.y() - dRemainHeight2,
			dRemainWidth2 * 2, dRemainHeight2 * 2);
	}
	this->setRect(rect);
	//�ı��С����Բ���жϣ����������ӻ���������
	//���
	if (dynamic_cast<CircleItem*>(parentItem()) != nullptr) {
		if (boundingRect().contains(QPointF(parentItem()->boundingRect().left(),parentItem()->boundingRect().center().y())))
		{
			QGraphicsItem* g = parentItem()->parentItem();
			QGraphicsItem* p = parentItem();
			p->setParentItem(0);
			//�����������
			QPointF itemPos = this->pos();
			this->setPos(parentItem()->pos());
			p->setPos(itemPos);
			setParentItem(g);
			p->setParentItem(this);
			p->setFlag(QGraphicsItem::ItemIsMovable, false);
			this->setFlag(QGraphicsItem::ItemIsMovable);
		}
	}
	//��С
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

