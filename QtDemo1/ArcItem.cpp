#include "ArcItem.h"


ArcItem::ArcItem(const QRectF& rectF, const qreal& startAngle, const qreal& spanAngle, const bool& pointFlag)
{
	this->rect = rectF;
	renewBoundRect();
	this->Radius = rectF.width() / 2;
	this->midPoint = rectF.center();
	this->startAngle = startAngle;
	this->spanAngle = spanAngle;
	this->pointFlag = pointFlag;
	this->arcsFlag = false;
}

ArcItem::ArcItem(const QRectF& rectF, const qreal& startAngle, const qreal& spanAngle, const qreal& radius, const bool& pointFlag)
{
	this->setAcceptHoverEvents(true);
	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->rect = rectF;
	renewBoundRect();
	this->midPoint = rectF.center();
	this->startAngle = startAngle;
	this->spanAngle = spanAngle;
	this->radius = radius;
	this->Radius = rectF.width() / 2;
	this->pointFlag = pointFlag;
	UpdateHandlesArea();
}

ArcItem::ArcItem(const ArcItem& arcItem, const bool& pointFlag)
{
	this->setAcceptHoverEvents(true);
	this->setFlag(QGraphicsItem::ItemIsMovable);
	this->setFlag(QGraphicsItem::ItemIsSelectable);
	this->rect = arcItem.rect;
	renewBoundRect();
	this->midPoint = arcItem.midPoint;
	this->startAngle = arcItem.startAngle;
	this->spanAngle = arcItem.spanAngle;
	this->radius = arcItem.radius;
	this->Radius = arcItem.Radius;
	this->pointFlag = pointFlag;
	UpdateHandlesArea();
}

ArcItem::~ArcItem()
{
	
}

QRectF ArcItem::boundingRect() const
{
	return boundRect;
}

void ArcItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
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
	//更新标点
	if (pointFlag) {
		//画出固定标记点
		UpdateHandlesArea();
		for (int i = 0; i < mHandleNum - 2; i++) {//还有旋转点位
			pItem = new PointItem(mouseHandles[i]);
			pItem->setParentItem(this);
			itemList.push_back(pItem);
			pItem = nullptr;
		}
	}
	//画弧线
	painter->setPen(pen);
	//必须保证Radius已初始化；
	painter->drawArc(QRectF(midPoint.x() - Radius, midPoint.y() - Radius, 2 * Radius, 2 * Radius), startAngle, spanAngle);
	if (arcsFlag) {
		//圆环
		painter->drawArc(QRectF(midPoint.x() - radius, midPoint.y() - radius, 2 * radius, 2 * radius), startAngle, spanAngle);
 		painter->drawLine(QLineF(mouseHandles[raRight].center(), mouseHandles[mRaRight].center()));
		painter->drawLine(QLineF(mouseHandles[raLeft].center(), mouseHandles[mRaLeft].center()));
	}
	if (this->isSelected()) {
		//绘制旋转点,依据middian
		painter->setPen(penCir);
		painter->setBrush(penCir.color());
		if(Radius >= radius)
			painter->drawLine(QLineF(mouseHandles[mRaMiddle].center(), mouseHandles[mReverseMiddle].center()));
		else
			painter->drawLine(QLineF(mouseHandles[raMiddle].center(), mouseHandles[mReverseMiddle].center()));
		painter->drawEllipse(mouseHandles[mReverseMiddle]);
	}

}

void ArcItem::setPen(QPen p)
{
	this->pen = p;
}

void ArcItem::renewRect()
{
	qreal maxRadius = radius > Radius ? radius : Radius;
	rect.setRect(midPoint.x() - maxRadius, midPoint.y() - maxRadius, 2 * maxRadius, 2 * maxRadius);
	renewBoundRect();
	this->update();
}

void ArcItem::renewBoundRect()
{
	boundRect.setRect(rect.left() - moreSize, rect.top() - moreSize, rect.width() + 2*moreSize, rect.height() + 2*moreSize);
}

void ArcItem::UpdateHandlesArea()
{
	//外圆
	mouseHandles[mRaRight] = QRectF(midPoint.x() + Radius*cos(startAngle/16*PI/180) - mHandlePointSize/2, 
		midPoint.y() - Radius*sin(startAngle/16*PI/180) - mHandlePointSize/2, mHandlePointSize, mHandlePointSize);
	mouseHandles[mRaLeft] = QRectF(midPoint.x() + Radius * cos((startAngle + spanAngle) / 16 * PI / 180) - mHandlePointSize / 2,
		midPoint.y() - Radius * sin((startAngle + spanAngle) / 16 * PI / 180) - mHandlePointSize / 2, mHandlePointSize, mHandlePointSize);
	mouseHandles[mRaMiddle] = QRectF(midPoint.x() + Radius * cos((startAngle + spanAngle/2) / 16 * PI / 180) - mHandlePointSize / 2,
		midPoint.y() - Radius * sin((startAngle + spanAngle/2) / 16 * PI / 180) - mHandlePointSize / 2, mHandlePointSize, mHandlePointSize);
	//内圆
	mouseHandles[raRight] = QRectF(midPoint.x() + radius * cos(startAngle / 16 * PI / 180) - mHandlePointSize / 2,
		midPoint.y() - radius * sin(startAngle / 16 * PI / 180) - mHandlePointSize / 2, mHandlePointSize, mHandlePointSize);
	mouseHandles[raLeft] = QRectF(midPoint.x() + radius * cos((startAngle + spanAngle) / 16 * PI / 180) - mHandlePointSize / 2,
		midPoint.y() - radius * sin((startAngle + spanAngle) / 16 * PI / 180) - mHandlePointSize / 2, mHandlePointSize, mHandlePointSize);
	mouseHandles[raMiddle] = QRectF(midPoint.x() + radius * cos((startAngle + spanAngle / 2) / 16 * PI / 180) - mHandlePointSize / 2,
		midPoint.y() - radius * sin((startAngle + spanAngle / 2) / 16 * PI / 180) - mHandlePointSize / 2, mHandlePointSize, mHandlePointSize);
	//旋转点
	if(Radius >= radius)
		mouseHandles[mReverseMiddle] = QRectF(midPoint.x() + (Radius + gap) * cos((startAngle + spanAngle / 2) / 16 * PI / 180) - reverCircleR,
			midPoint.y() - (Radius + gap) * sin((startAngle + spanAngle / 2) / 16 * PI / 180) - reverCircleR, 2 * reverCircleR, 2 * reverCircleR);
	else
		mouseHandles[mReverseMiddle] = QRectF(midPoint.x() + (radius + gap) * cos((startAngle + spanAngle / 2) / 16 * PI / 180) - reverCircleR,
			midPoint.y() - (radius + gap) * sin((startAngle + spanAngle / 2) / 16 * PI / 180) - reverCircleR, 2 * reverCircleR, 2 * reverCircleR);
}

ArcItem::mouseArcHandles ArcItem::GetMouseHandle(QPointF pointF)
{
	UpdateHandlesArea();
	mouseArcHandles mouseHandle;
	if (mouseHandles[mRaRight].contains(pointF))
		mouseHandle = mRaRight;
	else if (mouseHandles[mRaLeft].contains(pointF))
		mouseHandle = mRaLeft;
	else if (mouseHandles[mRaMiddle].contains(pointF))
		mouseHandle = mRaMiddle;
	else if (mouseHandles[raRight].contains(pointF))
		mouseHandle = raRight;
	else if (mouseHandles[raLeft].contains(pointF))
		mouseHandle = raLeft;
	else if (mouseHandles[raMiddle].contains(pointF))
		mouseHandle = raMiddle;
	else if (mouseHandles[mReverseMiddle].contains(pointF) && this->isSelected())
		mouseHandle = mReverseMiddle;
	else
		mouseHandle = noMouse;//还有旋转点位
	return mouseHandle;
}

void ArcItem::setMouseCursor(QGraphicsSceneHoverEvent* event)
{
	mouseHandle = GetMouseHandle(event->pos());
	if (mouseHandle != noMouse) {
		if (mouseHandle == mReverseMiddle && this->isSelected()) {
			setCursor(Qt::CrossCursor);
			return;
		}
		qreal angle;
		if(mouseHandle==mRaMiddle||mouseHandle==raMiddle)//中间点为依据垂直线
			angle = QLineF(midPoint, event->pos()).normalVector().angle();
		else
			angle = QLineF(midPoint, event->pos()).angle();
		//依据点所在半径不同角度来确定
		if ((angle >= 0 && angle < 23) || (angle >= 338 && angle < 360) || (angle >= 158 && angle < 203))
			setCursor(Qt::SizeVerCursor);
		else if ((angle >= 23 && angle < 68) || (angle >= 203 && angle < 248))
			setCursor(Qt::SizeFDiagCursor);
		else if ((angle >= 68 && angle < 113) || (angle >= 248 && angle < 293))
			setCursor(Qt::SizeHorCursor);
		else if ((angle >= 113 && angle < 158) || (angle >= 293 && angle < 338))
			setCursor(Qt::SizeBDiagCursor);
	}else
		setCursor(Qt::SizeAllCursor);
}

void ArcItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	return QGraphicsItem::hoverEnterEvent(event);
}

void ArcItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	setCursor(Qt::ArrowCursor);
	this->setSelected(false);
	return QGraphicsItem::hoverLeaveEvent(event);
}

void ArcItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
	setMouseCursor(event);
	return QGraphicsItem::hoverMoveEvent(event);
}

void ArcItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	pressPoint = event->pos();
	return QGraphicsItem::mousePressEvent(event);
}

void ArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	return QGraphicsItem::mouseReleaseEvent(event);
}

void ArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	UpdateHandlesArea();
	if (!(event->buttons() & Qt::LeftButton))
		return;
	this->prepareGeometryChange();
	//圆弧左边拖动处理
	if (mouseHandle == raLeft || mouseHandle == mRaLeft) 
	{
		qreal angle = QLineF(midPoint, mouseHandles[raLeft].center()).angleTo(QLineF(midPoint, event->pos()));
		//向圆弧内方向
		if (angle > 180)
			angle -= 360;
		spanAngle += angle*16;
		//限制转角范围
		if (spanAngle >= 360 * 16)
			spanAngle -= 360 * 16;
		if (spanAngle <= -360 * 16)
			spanAngle += 360 * 16;
		this->update();
		return;
	}else if (mouseHandle == raRight || mouseHandle == mRaRight)
	{
		qreal angle = QLineF(midPoint, mouseHandles[raRight].center()).angleTo(QLineF(midPoint, event->pos()));
		//向圆弧内方向
		if (angle > 180)
			angle -= 360;
		startAngle += angle * 16;
		spanAngle -= angle * 16;//保证圆弧左边不动
		//限制角范围
		if (startAngle >= 360 * 16)
			startAngle -= 360 * 16;
		if (startAngle <= -360 * 16)
			startAngle += 360 * 16;
		if (spanAngle >= 360 * 16)
			spanAngle -= 360 * 16;
		if (spanAngle <= -360 * 16)
			spanAngle += 360 * 16;
		this->update();
		return;
	}else if (mouseHandle == raMiddle)
	{//这里混淆了内圆，与外圆，以方便
		radius = QLineF(midPoint, event->pos()).length();
		renewRect();
		this->update();
		return;
	}else if (mouseHandle == mRaMiddle)
	{
		Radius = QLineF(midPoint, event->pos()).length();
		renewRect();
		this->update();
		return;
	}
	else if (mouseHandle == mReverseMiddle && this->isSelected())
	{//旋转
		// 设置中心点为原点
		QPointF originPos = this->rect.center();
		// 从原点延伸出去两条线，鼠标按下时的点和当前鼠标位置所在点的连线
		QLineF p1 = QLineF(originPos, pressPoint);
		QLineF p2 = QLineF(originPos, event->pos());
		// 旋转角度
		qreal dRotateAngle = p2.angleTo(p1);
		// 设置旋转中心
		this->setTransformOriginPoint(originPos);
		// 计算当前旋转的角度
		qreal dCurAngle = this->rotation() + dRotateAngle;
		while (dCurAngle > 360.0) {
			dCurAngle -= 360.0;
		}
		// 设置旋转角度
		this->setRotation(dCurAngle);
		// 刷新显示
		this->update();
		return;

	}
	
	return QGraphicsItem::mouseMoveEvent(event);
}