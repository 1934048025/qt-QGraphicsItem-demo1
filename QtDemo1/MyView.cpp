#include "MyView.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsItem>
#include <QDebug>
#include <QPointF>
MyView::MyView(QWidget* parent /* = 0 */):
	QGraphicsView(parent)
{
	
}

//鼠标滚轮
void MyView::wheelEvent(QWheelEvent* event)
{
	if (event->delta() > 0)
	{
		this->scale(1.3, 1.3);
		
	}
	else
	{
		this->scale(1 / 1.3, 1 / 1.3);
		
	}
	QGraphicsView::wheelEvent(event);
}



//比GraphicItem的鼠标高悬更新更快，但QMouseEvent会覆盖图形项的拖动??
// void MyView::mouseMoveEvent(QMouseEvent* event)
// {
// 	QPoint viewPos = event->pos();
// 	QPointF scenePos = mapToScene(viewPos);
// 	QGraphicsItem* item = scene()->itemAt(scenePos, QTransform());
// 	if (item) {
// 		QPointF itemPos = item->mapFromScene(scenePos);
// 		qDebug() << "itemPos:" << itemPos;
// 		QPoint point = itemPos.toPoint();
// 		QString str = "( " + QString::number(point.x())
// 			+ ", " + QString::number(point.y()) + ")";
// 		setToolTip(str);
// 	}
// 
// }