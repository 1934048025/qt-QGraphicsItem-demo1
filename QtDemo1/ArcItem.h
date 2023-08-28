#pragma once
#include <qgraphicsitem.h>
#include <QPen>
#include <QPainter>
#include <math.h>
#include <QGraphicsScene>
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include "PointItem.h"
class ArcItem :
    public QGraphicsItem
{
public:
    //只画弧线
    ArcItem(const QRectF& rectF, const qreal& startAngle, const qreal& spanAngle, const bool &pointFlag = true);
    //画圆弧
    ArcItem(const QRectF& rectF, const qreal& startAngle, const qreal& spanAngle, const qreal& radius, const bool &pointFlag = true);
    ArcItem(const ArcItem& arcItem, const bool& pointFlag = true);
    virtual ~ArcItem();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = nullptr */);
    void setPen(QPen p);
    void renewRect();
    void renewBoundRect();
protected:
    enum mouseArcHandles {
        mRaLeft,    //外圆， startAngle + spanAngle
        mRaRight,   //外圆， startAngle
        mRaMiddle,  //外圆中心
        raLeft, //内圆， startAngle + spanAngle
        raRight,    //内圆， startAngle
        raMiddle,  //内圆中心
        mReverseMiddle, //旋转点
        noMouse
    }mouseHandle;
	void UpdateHandlesArea();
	mouseArcHandles GetMouseHandle(QPointF pointF);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
	void setMouseCursor(QGraphicsSceneHoverEvent* event);

private:
	QPen pen = QPen(Qt::blue, 2);//画笔
    //边界矩形，圆心， 起始角度，旋转角度
    QRectF rect;
    QPointF midPoint;
    qreal startAngle;//已*16
    qreal spanAngle;
    qreal radius;//内圆半径
    qreal Radius;//外圆半径
    bool pointFlag = true;//是否需要标记标点
    bool arcsFlag = true;//是否是圆弧，画弧线不必标记固定点，即儿子
    const qreal PI = 3.1415926;
    //标记点
    QRectF mouseHandles[8];//固定的8的标记点范围
    int mHandleNum = 8;//鼠标标记的形态数量
    qreal mHandlePointSize = 6;//标记点宽高
	//标记点的绘制
	PointItem* pItem = nullptr;
	QVector<PointItem*> itemList;
    //  旋转
	qreal reverCircleR = 3;//旋转圆的半径
	qreal gap = 16;//旋转圆到矩形的距离
	qreal moreSize = reverCircleR + gap;//细节处理bounding的变化
	QPen penCir = QPen(Qt::red, 2);//旋转圆的
	QRectF boundRect;//区别父类的m_rect，这个是对其通过moreSize进行边界矩阵的增加。
	QPointF pressPoint;//记录鼠标按下的点,旋转

};

