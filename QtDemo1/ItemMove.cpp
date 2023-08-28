#include "ItemMove.h"
#include <QCursor>

ItemMove::ItemMove()
{
	setFlag(QGraphicsItem::ItemIsFocusable);
	setFlag(QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);
}


void ItemMove::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	setFocus();
	setCursor(Qt::ClosedHandCursor);
	return QGraphicsItem::mousePressEvent(event);
}

void ItemMove::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	setCursor(Qt::OpenHandCursor);
	return QGraphicsItem::hoverEnterEvent(event);
}

void ItemMove::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	setCursor(Qt::OpenHandCursor);
	return QGraphicsItem::mouseReleaseEvent(event);
}