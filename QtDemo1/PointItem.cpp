#include "PointItem.h"

PointItem::PointItem(const QRectF &r)
{
	mRect = r;
}

PointItem::PointItem(const QPointF& p, const qreal& x, const qreal& y)
{
	mRect.moveTopLeft(p);
	mRect.setWidth(x);
	mRect.setHeight(y);
}

QRectF PointItem::boundingRect() const
{
	return mRect;
}

void PointItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->fillRect(mRect, QBrush(m_color));
}

qreal PointItem::mouseHandleSize() const
{
	return m_dHandleSize;
}