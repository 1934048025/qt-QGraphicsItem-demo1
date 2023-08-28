#pragma once
#include <qgraphicsitem.h>
#include <QPainter>
class GraphicsItem :
    public QGraphicsItem
{
public:
	GraphicsItem();
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
private:
	
};

