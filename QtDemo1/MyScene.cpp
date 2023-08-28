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
	//����ͼ����
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
	return QGraphicsScene::mousePressEvent(event);//��֤���Ա��ؼ�����
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	//ʵʱ���ڹ����е�ͼ����ؼ�
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

//��Բ�¼�
void MyScene::mPressEventCircle(QGraphicsSceneMouseEvent* event)
{
	mPressEventPoint(event);//�������㲢��¼
	if (pressPoint.size() == 3 && (event->buttons() & Qt::LeftButton)) {
		//��ɻ�Բ
		newItem = false;
		if (rItem != nullptr)
		{
			removeItem(rItem);
			rItem = new CircleItem(rItem->boundingRect(), true);
		}
		rItem->setParentItem(gItem);
		emit newItemAchieve(rItem);
		//��ձ�ǵĵ�
		clearPoint();
		pressPoint.clear();
	}
}

void MyScene::mMoveEventCircle(QGraphicsSceneMouseEvent* event)
{
	//���㹹��һ��Բ

	if (pressPoint.size() == 2) {
		//����

		if (rItem != nullptr) {

			removeItem(rItem);

			//delete rItem; ���removeItem,������ָ��QScopedPointer<T, Cleanup>�ӹܣ�û�з���Ȩ��
			rItem = nullptr;
		}
		//��������ƶ��ĵ�
		pointF = mEventMovingPoint(event);
		//��Բ
		R = 0;
		midPoint = threePointSetCircle(pressPoint[0], pressPoint[1], pointF, R);
		rItem = new CircleItem(QRectF(midPoint.x() - R, midPoint.y() - R, 2 * R, 2 * R), false);
		rItem->setParentItem(gItem);
	}
}

//��Բ���¼�
void MyScene::mPressEventRing(QGraphicsSceneMouseEvent* event)
{
	mPressEventPoint(event);//�������㲢��¼
	if (pressPoint.size() == 3 && (event->buttons() & Qt::LeftButton)) {
		//��ɻ���һ��Բ,��Ϊ����Բ
		if (rItem != nullptr)
		{
			removeItem(rItem);
			rItem = new CircleItem(rItem->boundingRect(), true);
			rItem->setFlag(QGraphicsItem::ItemIsMovable, false);//��ֹ�ڻ滭ʱ�϶�
			RItem = rItem;
			rItem = nullptr;//��ֹ���ڶ���Բ��moveEvent���ͷŵ�
		}
		RItem->setParentItem(gItem);
		//��ձ�ǵĵ�
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
		//����Բ��Ϊ����Բ��СԲΪ��Բ
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
		//��ձ�ǵĵ�
		clearPoint();
		pressPoint.clear();
	}
}

void MyScene::mMoveEventRing(QGraphicsSceneMouseEvent* event)
{
	//����һ��Բ
	mMoveEventCircle(event);
	//���ڶ���Բ
	//���㹹��һ��Բ
	if (pressPoint.size() == 3) {
		//����
		if (rItem != nullptr) {
			removeItem(rItem);
			//delete rItem; ���removeItem,������ָ��QScopedPointer<T, Cleanup>�ӹܣ�û�з���Ȩ��
			rItem = nullptr;
		}
		//��������ƶ��ĵ�
		pointF = mEventMovingPoint(event);
		//��Բ
		r = 0;
		midPoint = twoPointSetCircle(midPoint, pointF, r);
		rItem = new CircleItem(QRectF(midPoint.x() - r, midPoint.y() - r, 2 * r, 2 * r), false);
		rItem->setParentItem(gItem);
	}
}

//��Բ��
void MyScene::mPressEventArc(QGraphicsSceneMouseEvent* event)
{
	//��ǲ���¼�����
	mPressEventPoint(event);
	if (pressPoint.size() == 4) {
		//��ɹ�������Բ��
		RItem = new ArcItem(*dynamic_cast<ArcItem*>(rItem), true);
		if (rItem != nullptr) {
			this->removeItem(rItem);
			rItem = nullptr;
		}
		newItem = false;
		this->gItem->setCursor(Qt::ArrowCursor);
		emit newItemAchieve(RItem);
		//��ձ�ǵĵ�
		clearPoint();
		pressPoint.clear();
	}
}

void MyScene::mMoveEventArc(QGraphicsSceneMouseEvent* event)
{
	//��һ����
	if (pressPoint.size() == 2) {
		if (rItem != nullptr) {
			removeItem(rItem);
			rItem = nullptr;
		}
		//������ƶ��ĵ�
		pointF = mEventMovingPoint(event);
		R = 0;
		midPoint = threePointSetCircle(pressPoint[0], pressPoint[1], pointF, R);
		//��ʼ�Ƕ�
		linesF[0] = QLineF(midPoint, pressPoint[0]);
		linesF[1] = QLineF(midPoint, pressPoint[1]);
		linesF[2] = QLineF(midPoint, pointF);
		if (linesF[0].angle() > linesF[1].angle()) {
			QLineF mid = linesF[0];
			linesF[0] = linesF[1];
			linesF[1] = mid;
		}
		startAngle = 16*linesF[0].angle();//��Ϊ0~pi/2
		if ((linesF[0].angleTo(linesF[2])) + (linesF[2].angleTo(linesF[1])) <= 360)//��Բ��Ϊ��ʱ�룬����Ϊ˳
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
	//ȷ���߽�������ھ�
	if (pressPoint.size() == 3) {
		if (rItem != nullptr) {
			removeItem(rItem);
			rItem = nullptr;
		}
		//������ƶ��ĵ�
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

//�������
void MyScene::mPressEventPloyline(QGraphicsSceneMouseEvent* event)
{
	//��ǵ㲢����
	mPressEventPoint(event);
	int size = pressPoint.size();
	if (size > 1 && (event->buttons() & Qt::LeftButton)) {
		QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(pressPoint[size - 2], pressPoint[size - 1]));
		lineItem->setPen(pen);
		graphicsItems.push_back(lineItem);
		lineItem->setParentItem(gItem);
	}
	//�Ҽ��Զ��������ƶ����
	if (size >= 3 && (event->buttons() & Qt::RightButton)) {
		QGraphicsLineItem* lineItem = new QGraphicsLineItem(QLineF(pressPoint[size - 1], pressPoint[0]));
		lineItem->setPen(pen);
		graphicsItems.push_back(lineItem);
		lineItem->setParentItem(gItem);
		//��һ�������Ķ������
		if (rItem != nullptr) {
			this->removeItem(rItem);
			rItem = nullptr;
		}
		RItem = new PolygonItem(pressPoint);
		newItem = false;
		this->gItem->setCursor(Qt::ArrowCursor);
		emit newItemAchieve(RItem);
		//��ձ�ǵĵ�
		clearPoint();
		pressPoint.clear();
		//��ռ�¼����

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
		//����ƶ��ĵ�
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
	//���mouseMoveEvent���±�ǵĵ�mSItem
	//�����¼
	if (!(event->buttons() & Qt::LeftButton))
		return;
	if (mSItem != nullptr) {
		removeItem(mSItem);
		delete mSItem;
		mSItem = nullptr;
	}
	//���һ����
	pointF = event->scenePos();
	pointF = gItem->mapFromScene(pointF);
	qDebug() << event->scenePos();
	pressPoint.push_back(pointF);
	mSItem = new PointItem(pointF);
	mSItem->setZValue(1);
	//���ø���������Ҳ����Ը��׽�����
	mSItem->setParentItem(gItem);
	pointItemList.push_back(mSItem);
	mSItem = nullptr;
}

QPointF MyScene::mEventMovingPoint(QGraphicsSceneMouseEvent* event)
{
	QPointF pointF = QPointF(event->scenePos());
	pointF = gItem->mapFromScene(pointF);
	//��������ƶ��ĵ�
	if (mSItem != nullptr) {
		removeItem(mSItem);
		delete mSItem;
		mSItem = nullptr;
	}
	mSItem = new PointItem(pointF);
	mSItem->setZValue(1);
	mSItem->setParentItem(gItem);//������������
	return pointF;
}

void MyScene::clearPoint()
{
	//��ձ�ǵĵ�
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