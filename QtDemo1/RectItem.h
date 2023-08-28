#pragma once
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "PointItem.h"
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QString>


class RectItem :
    public QGraphicsItem
{
public:
    RectItem();
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
protected:
	enum mouseHandle
	{
		Mouse_LeftTop,
		Mouse_RightTop,
		Mouse_LeftBottom,
		Mouse_RightBottom,
		Mouse_MiddleTop,
		Mouse_MiddleBottom,
		Mouse_MiddleLeft,
		Mouse_MiddleRight,
		Mouse_Reverse,//用于旋转矩阵及椭圆
		Mouse_None
	}rectMouseHandle;

	virtual void UpdateHandlesArea();
	virtual mouseHandle GetMouseHandle(QPointF pointF);
	virtual void setMouseCursor(QGraphicsSceneHoverEvent* event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void ResizeRect(const QPointF& mousePos);
	
	virtual void setRect(const QRectF& rect) {
		m_rect = rect; this->update();
		qDebug() << QString("rect  width: %1  height: %2").arg(rect.width()).arg(rect.height());
	}
	

	const QPen pen = QPen(Qt::blue, 2);//画笔

	const double m_dHandleSize = 6; //鼠标变形的范围
	QRectF m_handlesArea[10];//鼠标形态
	int mouseNum = 10;

	// 需要绘制初始的矩形
	QRectF m_rect = QRectF(20, 20, 300, 200);

	// 绘制标志，防止第一次鼠标拖动绘制时，图元的鼠标移动事件触发，使得图元移动
	//这里矩形未使用拖动式绘图
	bool m_bDrawFlag = false;

	//中间变量
	QRectF m_pressRect;
	const qreal m_dAdjust = 1;
	const qreal dMixSize = 2.0 * (1 + m_dAdjust);   // 收缩时的最小尺寸
	//标记点的绘制
	PointItem* pItem = nullptr;
	std::vector<PointItem*> itemList;
private:
	
};

