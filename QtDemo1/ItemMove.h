#pragma once
#include <qgraphicsitem.h>
//Œ¥∆Ù”√
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

