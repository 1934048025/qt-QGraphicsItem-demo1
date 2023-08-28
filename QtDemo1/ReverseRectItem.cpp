#include "reverseRectItem.h"
#include <QPainter>
ReverseRectItem::ReverseRectItem()
{
	// ������������¼�
	this->setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);//ѡ������ʾ��תԲ
	renewBoundRect();
}

QRectF ReverseRectItem::boundingRect() const
{
	qreal adjust = 0.5;
	return boundRect;
}

void ReverseRectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	RectItem::paint(painter, option, widget);
	//����ͷ
	painter->setPen(pen);
	qreal midx = m_rect.width() / 100;
	qreal midy = m_rect.height() / 100;
	midx *= 2;
	midy *= 2;
	QPoint mid = QPoint(m_rect.x() + m_rect.width()/2, m_rect.y() + m_rect.height()/2);
	painter->drawLine(QLineF(QPoint(m_rect.x() + 0, m_rect.y() + m_rect.height() / 2), mid));
	painter->drawLine(QLineF(QPoint(mid.x() - midx, mid.y() - midy), mid));
	painter->drawLine(QLineF(QPoint(mid.x() - midx, mid.y() + midy), mid));
	if (this->isSelected()) {
		//������ת��
		painter->setPen(penCir);
		painter->setBrush(penCir.color());
		painter->setBrush(penCir.color());
		painter->drawLine(QLineF(m_rect.center().x(), m_rect.top(), m_rect.center().x(), m_rect.top() - gap));
		painter->drawEllipse(QRectF(m_rect.center().x() - reverCircleR, m_rect.top() - gap - reverCircleR, 2 * reverCircleR, 2 * reverCircleR));
	}
}

void ReverseRectItem::renewBoundRect()
{
	boundRect.setRect(m_rect.left(), m_rect.top() - moreSize, m_rect.width(), m_rect.height() + moreSize);
}

void ReverseRectItem::setRect(const QRectF& rect)
{
	m_rect = rect;
	renewBoundRect();
	this->update();
}

void ReverseRectItem::UpdateHandlesArea()
{
	RectItem::UpdateHandlesArea();
	m_handlesArea[Mouse_Reverse] = QRectF(boundingRect().center().x() - reverCircleR, boundingRect().top() , 2*reverCircleR, 2*reverCircleR);

}

ReverseRectItem::mouseHandle ReverseRectItem::GetMouseHandle(QPointF pointF)
{
	UpdateHandlesArea();
	if (m_handlesArea[Mouse_Reverse].contains(pointF))
		return Mouse_Reverse;
	else
		return RectItem::GetMouseHandle(pointF);
	return Mouse_None;
}

void ReverseRectItem::setMouseCursor(QGraphicsSceneHoverEvent* event)
{
	RectItem::setMouseCursor(event);
	if (rectMouseHandle == Mouse_Reverse && this->isSelected())
		this->setCursor(Qt::CrossCursor);//Ӧ�õ����Զ������תͼ��
}

void ReverseRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	pressPoint = event->pos();
	return RectItem::mousePressEvent(event);
}

void ReverseRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		if (rectMouseHandle == Mouse_Reverse && this->isSelected()) {
			// �������ĵ�Ϊԭ��
			QPointF originPos = this->m_rect.center();
			// ��ԭ�������ȥ�����ߣ���갴��ʱ�ĵ�͵�ǰ���λ�����ڵ������
			QLineF p1 = QLineF(originPos, pressPoint);
			QLineF p2 = QLineF(originPos, event->pos());
			// ��ת�Ƕ�
			qreal dRotateAngle = p2.angleTo(p1);
			// ������ת����
			this->setTransformOriginPoint(originPos);
			// ���㵱ǰ��ת�ĽǶ�
			qreal dCurAngle = this->rotation() + dRotateAngle;
			while (dCurAngle > 360.0) {
				dCurAngle -= 360.0;
			}
			// ������ת�Ƕ�
			this->setRotation(dCurAngle);
			// ˢ����ʾ
			this->update();
			return;
		}
	}
	RectItem::mouseMoveEvent(event);
}

void ReverseRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	//������ת������ĵ��µ���ת����λ��ƫ��
	if (event->button() == Qt::LeftButton && 	// �������
		rectMouseHandle != Mouse_None)  	// ֻ�����϶�����
	{
		// �������ת�Ƕȵľ��Σ�����֮���ٴ���ת����ת���ĸ���ȻΪ֮ǰ���꣬�ֶ�����Ϊ���ģ������Ư�Ƶ�����
		auto rr = this->m_rect;
		auto angle = rotation()*PI/180;
		auto p1 = rr.center();
		auto origin = this->transformOriginPoint();
		QPointF p2 = QPointF(0, 0);

		p2.setX(origin.x() + qCos(angle) * (p1.x() - origin.x()) - qSin(angle) * (p1.y() - origin.y()));
		p2.setY(origin.y() + qSin(angle) * (p1.x() - origin.x()) + qCos(angle) * (p1.y() - origin.y()));

		auto diff = p1 - p2;

		this->setRect(rr.adjusted(-diff.x(), -diff.y(), -diff.x(), -diff.y()));
		this->setTransformOriginPoint(this->m_rect.center());
		this->update();

		// �϶�������ָ�ѡ��
		this->setSelected(true);
	}
	return RectItem::mouseReleaseEvent(event);
}