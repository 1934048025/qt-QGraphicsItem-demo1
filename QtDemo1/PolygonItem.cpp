#include "PolygonItem.h"

PolygonItem::PolygonItem(QVector<QPointF> points)
{
	this->points = points;
	renewRect();
	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsMovable);
}

PolygonItem::~PolygonItem()
{

}

QRectF PolygonItem::boundingRect() const
{
	return rect;
}

void PolygonItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//清除
	while (itemList.size() > 0)
	{
		pItem = itemList[0];
		itemList.erase(itemList.begin());
		parentItem()->scene()->removeItem(pItem);
		delete pItem;
		pItem = nullptr;
	}
	//画标点
	for (auto t= points.begin(); t != points.end(); t++)
	{
		pItem = new PointItem(QRectF(t->x() - mPointHandleSize/2, t->y() - mPointHandleSize/2 , mPointHandleSize, mPointHandleSize));
		itemList.push_back(pItem);
		pItem->setParentItem(this);
		pItem = nullptr;
	}
	//画线
	painter->setPen(pen);
	for (auto t = points.begin() + 1; t != points.end(); t++)
	{
		painter->drawLine(QLineF(*(t - 1), *t));
	}
	painter->drawLine(QLineF(points[0], points[points.size() - 1]));
}

void PolygonItem::renewRect()
{
	rect = QGraphicsPolygonItem(QPolygonF(points)).boundingRect();
	update();
}

void PolygonItem::setPen(QPen p)
{
	pen = p;
}



void PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	return QGraphicsItem::mousePressEvent(event);
}

void PolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	return QGraphicsItem::mouseReleaseEvent(event);
}

void PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (mouseHandle == pointMouse) {
		this->prepareGeometryChange();
		points[index] = event->pos();
		renewRect();
		this->update();
		return;
	}
	return QGraphicsItem::mouseMoveEvent(event);
}

void PolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	return QGraphicsItem::hoverEnterEvent(event);
}

void PolygonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	this->setCursor(Qt::ArrowCursor);
	return QGraphicsItem::hoverLeaveEvent(event);
}
void PolygonItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	setMouseCursor(event);
	return QGraphicsItem::hoverMoveEvent(event);
}

//自动改变鼠标样式
void PolygonItem::setMouseCursor(QGraphicsSceneHoverEvent* event)
{
	for (index = 0; index < itemList.size(); index++) {
		if (itemList[index]->contains(event->pos())) {
			mouseHandle = pointMouse;
			this->setCursor(Qt::CrossCursor);
			return;
		}
	}
	this->setCursor(Qt::SizeAllCursor);
	mouseHandle = noMouse;
}