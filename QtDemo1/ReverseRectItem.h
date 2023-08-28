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

	qreal reverCircleR = 3;//旋转圆的半径
	qreal gap = 16;//旋转圆到矩形的距离
	qreal moreSize = reverCircleR + gap;//细节处理bounding的变化
	QPen penCir = QPen(Qt::red, 2);//旋转圆的
	QRectF boundRect;//区别父类的m_rect，这个是对其通过moreSize进行边界矩阵的增加。
	QPointF pressPoint;//记录鼠标按下的点,旋转
	const qreal PI = 3.1415926;
private:
};

