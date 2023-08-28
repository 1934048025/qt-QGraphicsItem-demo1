#include "GraphicsItem.h"
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPointF>
#include <QDebug>
GraphicsItem::GraphicsItem()
{
	//setFlag(QGraphicsItem::ItemIsFocusable);//聚焦，接受键盘事件
	//setFlag(QGraphicsItem::ItemIsSelectable);//可选择，鼠标点击或设置选择
	setFlag(QGraphicsItem::ItemIsMovable);//鼠标可拖动,但这里不知为何复位
	setAcceptHoverEvents(true);//类似Widget：：setMouseTracking(true),	hoverMoveEvent()
}

QRectF GraphicsItem::boundingRect() const
{
	qreal adjust = 0.5;
	return QRectF(0 - adjust, 0 - adjust,
		800 + adjust, 500 + adjust);
}

void GraphicsItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->drawImage(QRect(0, 0, 800, 500), QImage(":/QtDemo1/page1.PNG"));
	//图像的拖拽一直复位，但图形项却不会？
}

void GraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	setFocus();
	setCursor(Qt::ClosedHandCursor);
	return QGraphicsItem::mousePressEvent(event);
}


void GraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	//setCursor(Qt::OpenHandCursor);
	
	return QGraphicsItem::hoverEnterEvent(event);//有什么作用？同下
}

void GraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	//显示坐标
	QPointF scenePos = event->scenePos();
	QPointF itemPos = mapFromScene(scenePos);
	QPoint point = itemPos.toPoint();
	QString str = "( " + QString::number(point.x())
		+ ", " + QString::number(point.y()) + ")";
	setToolTip(str);
	//qDebug() << str;//降低延迟
	return QGraphicsItem::hoverMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	//鼠标恢复默认
	setCursor(Qt::ArrowCursor);
	return QGraphicsItem::mouseReleaseEvent(event);//避免覆盖基类的内容，这里会存储图形类移动的锚点
}
