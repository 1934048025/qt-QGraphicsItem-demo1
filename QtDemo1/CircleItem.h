#pragma once
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsScene>
#include "PointItem.h"


class CircleItem :
	public QGraphicsItem
{
public:
	CircleItem();
	CircleItem(QRectF r, bool pItemFlag = true);
	virtual ~CircleItem();
	qreal radius() const;
protected:
	enum mouseHandleCircle
	{
		Mouse_MiddleTop,
		Mouse_MiddleBottom,
		Mouse_MiddleLeft,
		Mouse_MiddleRight,
		Mouse_None
	}circleMouseHandle;

	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void UpdateHandlesArea();
	mouseHandleCircle GetMouseHandle(QPointF pointF);
	void ResizeRect(const QPointF& mousePos);
	void setMouseCursor(QGraphicsSceneHoverEvent* event);
	QPoint getMiddlePoint();
	void setRect(const QRectF& rect) {
		m_rect = rect;
		//middleLon = m_rect.width() / 50;
		this->update();
		qDebug() << QString("rect  width: %1  height: %2").arg(rect.width()).arg(rect.height());
	}

private:
	const QPen pen = QPen(Qt::blue, 2);//画笔
	const QPen penMiddle = QPen(Qt::red, 2);//圆心的画笔
	
	bool pointItemFlag = true;//判断是否要为圆的四周画上四个标记点
	const double m_dHandleSize = 6; //鼠标变形的范围
	const QColor m_color = QColor(Qt::green);//标记颜色
	QRectF m_handlesArea[5];//鼠标形态
	int mouseNum = 5;//形态数量
	// 需要绘制初始的矩形
	QRectF m_rect = QRectF(0, 0, 200, 200);
	qreal middleLon = 4; //m_rect.width() / 50;//圆中心标长度，伸缩绘制不标准

	// 绘制标志，防止第一次鼠标拖动绘制时，图元的鼠标移动事件触发，使得图元移动
	bool m_bDrawFlag = false;//未启用

	//中间变量
	QRectF m_pressRect;
	QPointF midPoint;//圆心
	const qreal m_dAdjust = 1;
	const qreal dMixSize = 2.0 * (1 + m_dAdjust);   // 收缩时的最小尺寸
	
	//标记点的绘制
	PointItem* pItem = nullptr;
	QVector<PointItem*> itemList;
};
