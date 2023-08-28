#include "MyScene.h"
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include <limits>
#include <stdio.h>

MyScene::MyScene()
{

}

void MyScene::setItemType(int index)
{
	if (index >= 0)
	{
		itemType = index;
	}
}

void MyScene::setNewItem()
{
	newItem = true;
}

void MyScene::setPen(QPen p)
{
	pen = p;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	//构建图形项
	if (newItem && (event->buttons())) {
		switch (itemType)
		{
		case Circle:	mPressEventCircle(event);break;
		case Ring:	mPressEventRing(event); break;
		case Arc:	mPressEventArc(event); break;
		case Ployline:	mPressEventPloyline(event);break;
		default:
			break;
		}
	}
	return QGraphicsScene::mousePressEvent(event);//保证可以被控件接收
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//实时正在构建中的图形项控件
	if (newItem)
	{
		switch (itemType)
		{
		case Circle:	mMoveEventCircle(event);break;
		case Ring:	mMoveEventRing(event); break;
		case Arc:	mMoveEventArc(event); break;
		case Ployline:	mMoveEventPloyline(event); break;
		default:
			break;
		}
	}

	return QGraphicsScene::mouseMoveEvent(event);
}

//画圆事件
void MyScene::mPressEventCircle(QGraphicsSceneMouseEvent* event)
{
	mPressEventPoint(event);//标记这个点并记录
	if (pressPoint.size() == 3 && (event->buttons() & Qt::LeftButton)) {
		//完成画圆
		newItem = false;
		if (rItem != nullptr)
		{
			removeItem(rItem);
			rItem = new CircleItem(rItem->boundingRect(), true);
		}
		rItem->setParentItem(gItem);
		emit newItemAchieve(rItem);
		//清空标记的点
		clearPoint();
		pressPoint.clear();
	}
}

void MyScene::mMoveEventCircle(QGraphicsSceneMouseEvent* event)
{
	//三点构建一个圆

	if (pressPoint.size() == 2) {
		//更新

		if (rItem != nullptr) {

			removeItem(rItem);

			//delete rItem; 这里，removeItem,被智能指针QScopedPointer<T, Cleanup>接管，没有访问权限
			rItem = nullptr;
		}
		//标记正在移动的点
		pointF = mEventMovingPoint(event);
		//画圆
		R = 0;
		midPoint = threePointSetCircle(pressPoint[0], pressPoint[1], pointF, R);
		rItem = new CircleItem(QRectF(midPoint.x() - R, midPoint.y() - R, 2 * R, 2 * R), false);
		rItem->setParentItem(gItem);
	}
}

//画圆环事件
void MyScene::mPressEventRing(QGraphicsSceneMouseEvent* event)
{
	mPressEventPoint(event);//标记这个点并记录
	if (pressPoint.size() == 3 && (event->buttons() & Qt::LeftButton)) {
		//完成画第一个圆,作为父亲圆
		if (rItem != nullptr)
		{
			removeItem(rItem);
			rItem = new CircleItem(rItem->boundingRect(), true);
			rItem->setFlag(QGraphicsItem::ItemIsMovable, false);//防止在绘画时拖动
			RItem = rItem;
			rItem = nullptr;//防止画第二个圆，moveEvent再释放掉
		}
		RItem->setParentItem(gItem);
		//清空标记的点
		clearPoint();
	}
	if (pressPoint.size() == 4 && (event->buttons() & Qt::LeftButton))
	{
		newItem = false;
		if (rItem != nullptr)
		{
			removeItem(rItem);
			rItem = new CircleItem(rItem->boundingRect(), true);
		}
		RItem->setParentItem(0);
		//将大圆作为父亲圆，小圆为子圆
		if (r > R) {
			qreal mid = r;
			r = R;
			R = mid;
			QGraphicsItem* midI = rItem;
			rItem = RItem;
			RItem = midI;
		}
		rItem->setFlag(QGraphicsItem::ItemIsMovable, false);//false
		RItem->setFlag(QGraphicsItem::ItemIsMovable);
		RItem->setParentItem(gItem);
		rItem->setParentItem(RItem);
		emit newItemAchieve(RItem);
		//清空标记的点
		clearPoint();
		pressPoint.clear();
	}
}

void MyScene::mMoveEventRing(QGraphicsSceneMouseEvent* event)
{
	//画第一个圆
	mMoveEventCircle(event);
	//画第二个圆
	//三点构建一个圆
	if (pressPoint.size() == 3) {
		//更新
		if (rItem != nullptr) {
			removeItem(rItem);
			//delete rItem; 这里，removeItem,被智能指针QScopedPointer<T, Cleanup>接管，没有访问权限
			rItem = nullptr;
		}
		//标记正在移动的点
		pointF = mEventMovingPoint(event);
		//画圆
		r = 0;
		midPoint = twoPointSetCircle(midPoint, pointF, r);
		rItem = new CircleItem(QRectF(midPoint.x() - r, midPoint.y() - r, 2 * r, 2 * r), false);
		rItem->setParentItem(gItem);
	}
}

//画圆弧
void MyScene::mPressEventArc(QGraphicsSceneMouseEvent* event)
{
	//标记并记录这个点
	mPressEventPoint(event);
	if (pressPoint.size() == 4) {
		//完成构建最后的圆弧
		RItem = new ArcItem(*dynamic_cast<ArcItem*>(rItem), true);
		if (rItem != nullptr) {
			this->removeItem(rItem);
			rItem = nullptr;
		}
		newItem = false;
		this->gItem->setCursor(Qt::ArrowCursor);
		emit newItemAchieve(RItem);
		//清空标记的点
		clearPoint();
		pressPoint.clear();
	}
}

void MyScene::mMoveEventArc(QGraphicsSceneMouseEvent* event)
{
	//第一条弧
	if (pressPoint.size() == 2) {
		if (rItem != nullptr) {
			removeItem(rItem);
			rItem = nullptr;
		}
		//标记在移动的点
		pointF = mEventMovingPoint(event);
		R = 0;
		midPoint = threePointSetCircle(pressPoint[0], pressPoint[1], pointF, R);
		//起始角度
		linesF[0] = QLineF(midPoint, pressPoint[0]);
		linesF[1] = QLineF(midPoint, pressPoint[1]);
		linesF[2] = QLineF(midPoint, pointF);
		if (linesF[0].angle() > linesF[1].angle()) {
			QLineF mid = linesF[0];
			linesF[0] = linesF[1];
			linesF[1] = mid;
		}
		startAngle = 16*linesF[0].angle();//设为0~pi/2
		if ((linesF[0].angleTo(linesF[2])) + (linesF[2].angleTo(linesF[1])) <= 360)//则圆弧为逆时针，否则为顺
			spanAngle = linesF[1].angle() - linesF[0].angle();
		else
			spanAngle = -360 + (linesF[1].angle() - linesF[0].angle());
		spanAngle *= 16;
		if (rItem != nullptr) {
			removeItem(rItem);
			rItem = nullptr;
		}
		rItem = new ArcItem(QRectF(midPoint.x() - R, midPoint.y() - R, 2 * R, 2 * R), startAngle, spanAngle, false);
		rItem->setParentItem(gItem);
	}
	//确定边界矩形与内径
	if (pressPoint.size() == 3) {
		if (rItem != nullptr) {
			removeItem(rItem);
			rItem = nullptr;
		}
		//标记在移动的点
		pointF = mEventMovingPoint(event);
		r = QLineF(midPoint, pointF).length();
		if (r <= R) {
			rItem = new ArcItem(QRectF(midPoint.x() - R, midPoint.y() - R, 2 * R, 2 * R), startAngle, spanAngle, r, false);
		}
		else {
			rItem = new ArcItem(QRectF(midPoint.x() - r, midPoint.y() - r, 2 * r, 2 * r), startAngle, spanAngle, R, false);
		}
		rItem->setParentItem(gItem);
	}
}	

//画多边形
void MyScene::mPressEventPloyline(QGraphicsSceneMouseEvent* event)
{
	//标记点并记入
	mPressEventPoint(event);
	int size = pressPoint.size();
	if (size > 1 && (event->buttons() & Qt::LeftButton)) {
		QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(pressPoint[size - 2], pressPoint[size - 1]));
		lineItem->setPen(pen);
		graphicsItems.push_back(lineItem);
		lineItem->setParentItem(gItem);
	}
	//右键自动连接完善多边形
	if (size >= 3 && (event->buttons() & Qt::RightButton)) {
		QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(pressPoint[size - 1], pressPoint[0]));
		lineItem->setPen(pen);
		graphicsItems.push_back(lineItem);
		lineItem->setParentItem(gItem);
		//画一个完整的多边形类
		if (rItem != nullptr) {
			this->removeItem(rItem);
			rItem = nullptr;
		}
		RItem = new PolygonItem(pressPoint);
		newItem = false;
		this->gItem->setCursor(Qt::ArrowCursor);
		emit newItemAchieve(RItem);
		//清空标记的点
		clearPoint();
		pressPoint.clear();
		//清空记录的线

		while (graphicsItems.size() > 0)
		{
			rItem = graphicsItems[0];
			graphicsItems.erase(graphicsItems.begin());
			removeItem(rItem);
			rItem = nullptr;
		}
	}
}

void MyScene::mMoveEventPloyline(QGraphicsSceneMouseEvent* event)
{
	this->gItem->setCursor(Qt::CrossCursor);
	if (pressPoint.size() > 0) {
		//标记移动的点
		pointF = mEventMovingPoint(event);
		if (rItem != nullptr)
		{
			this->removeItem(rItem);
			rItem = nullptr;
		}
		rItem = addLine(QLineF(pressPoint[pressPoint.size()-1], pointF), pen);
		rItem->setParentItem(gItem);
	}
}

void MyScene::mPressEventPoint(QGraphicsSceneMouseEvent* event)
{
	//清除mouseMoveEvent更新标记的点mSItem
	//左键记录
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if (mSItem != nullptr) {
		removeItem(mSItem);
		delete mSItem;
		mSItem = nullptr;
	}
	//标记一个点
	pointF = event->scenePos();
	pointF = gItem->mapFromScene(pointF);
	qDebug() << event->scenePos();
	pressPoint.push_back(pointF);
	mSItem = new PointItem(pointF);
	mSItem->setZValue(1);
	//设置父亲则坐标也是相对父亲建立的
	mSItem->setParentItem(gItem);
	pointItemList.push_back(mSItem);
	mSItem = nullptr;
}

QPointF MyScene::mEventMovingPoint(QGraphicsSceneMouseEvent* event)
{
	QPointF pointF = QPointF(event->scenePos());
	pointF = gItem->mapFromScene(pointF);
	//标记正在移动的点
	if (mSItem != nullptr) {
		removeItem(mSItem);
		delete mSItem;
		mSItem = nullptr;
	}
	mSItem = new PointItem(pointF);
	mSItem->setZValue(1);
	mSItem->setParentItem(gItem);//最后点击会清理掉
	return pointF;
}

void MyScene::clearPoint()
{
	//清空标记的点
	if (mSItem != nullptr) {
		removeItem(mSItem);
		delete mSItem;
		mSItem = nullptr;
	}
	while (pointItemList.size() > 0)
	{
		mSItem = pointItemList[0];
		pointItemList.erase(pointItemList.begin());
		removeItem(mSItem);
		delete mSItem;
		mSItem = nullptr;
	}
}

QPointF MyScene::twoPointSetCircle(QPointF midPoint, QPointF b, qreal& radius)
{
	radius = sqrt((midPoint.x() - b.x()) * (midPoint.x() - b.x())
		+ (midPoint.y() - b.y()) * (midPoint.y() - b.y()));
	return midPoint;
}

QPointF MyScene::threePointSetCircle(QPointF pt1, QPointF pt2, QPointF pt3, double& radius)
{
	double x1 = pt1.x(), x2 = pt2.x(), x3 = pt3.x();
	double y1 = pt1.y(), y2 = pt2.y(), y3 = pt3.y();
	double a = x1 - x2;
	double b = y1 - y2;
	double c = x1 - x3;
	double d = y1 - y3;
	double e = ((x1 * x1 - x2 * x2) + (y1 * y1 - y2 * y2)) / 2.0;
	double f = ((x1 * x1 - x3 * x3) + (y1 * y1 - y3 * y3)) / 2.0;
	double det = b * c - a * d;
	if (fabs(det) < 1e-5)
	{
		radius = -1;
		return QPointF(0, 0);
	}

	double x0 = -(d * e - b * f) / det;
	double y0 = -(a * f - c * e) / det;
	radius = hypot(x1 - x0, y1 - y0);
	return QPointF(x0, y0);
}



void MyScene::setGraphicsItem(QGraphicsItem* item)
{
	this->gItem = item;
}