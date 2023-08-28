#pragma once
#include "ReverseRectItem.h"
class EllipseItem :
    public ReverseRectItem
{
public:
	EllipseItem();
	QRectF boundingRect() const;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
private:
};

