#pragma once
#include <qgraphicsitem.h>
//δ����
class ItemMove :
    public QGraphicsItem
{
public:
	ItemMove();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
};

