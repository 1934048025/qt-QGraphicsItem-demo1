#pragma once
#include "RectItem.h"
#include <qmath.h>
class ReverseRectItem :
    public RectItem
{
public:
    ReverseRectItem();
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
protected:
	virtual void UpdateHandlesArea();
	virtual mouseHandle GetMouseHandle(QPointF pointF);
	virtual void setMouseCursor(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void renewBoundRect();
	virtual void setRect(const QRectF& rect);

	qreal reverCircleR = 3;//��תԲ�İ뾶
	qreal gap = 16;//��תԲ�����εľ���
	qreal moreSize = reverCircleR + gap;//ϸ�ڴ���bounding�ı仯
	QPen penCir = QPen(Qt::red, 2);//��תԲ��
	QRectF boundRect;//�������m_rect������Ƕ���ͨ��moreSize���б߽��������ӡ�
	QPointF pressPoint;//��¼��갴�µĵ�,��ת
	const qreal PI = 3.1415926;
private:
};

