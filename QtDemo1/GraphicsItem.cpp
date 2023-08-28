#include "GraphicsItem.h"
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QPointF>
#include <QDebug>
GraphicsItem::GraphicsItem()
{
	//setFlag(QGraphicsItem::ItemIsFocusable);//�۽������ܼ����¼�
	//setFlag(QGraphicsItem::ItemIsSelectable);//��ѡ�������������ѡ��
	setFlag(QGraphicsItem::ItemIsMovable);//�����϶�,�����ﲻ֪Ϊ�θ�λ
	setAcceptHoverEvents(true);//����Widget����setMouseTracking(true),	hoverMoveEvent()
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
	//ͼ�����קһֱ��λ����ͼ����ȴ���᣿
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
	
	return QGraphicsItem::hoverEnterEvent(event);//��ʲô���ã�ͬ��
}

void GraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	//��ʾ����
	QPointF scenePos = event->scenePos();
	QPointF itemPos = mapFromScene(scenePos);
	QPoint point = itemPos.toPoint();
	QString str = "( " + QString::number(point.x())
		+ ", " + QString::number(point.y()) + ")";
	setToolTip(str);
	//qDebug() << str;//�����ӳ�
	return QGraphicsItem::hoverMoveEvent(event);
}

void GraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	//���ָ�Ĭ��
	setCursor(Qt::ArrowCursor);
	return QGraphicsItem::mouseReleaseEvent(event);//���⸲�ǻ�������ݣ������洢ͼ�����ƶ���ê��
}
