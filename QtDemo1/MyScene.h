#pragma once
#include <qgraphicsscene.h>
#include "PointItem.h"
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QDebug>
#include "CircleItem.h"
#include "PolygonItem.h"
#include "ArcItem.h"
class MyScene :
    public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene();
    void setItemType(int index);
    void setGraphicsItem(QGraphicsItem* item);
    void setNewItem();//将通过鼠标点击构建新图形项，true
    QGraphicsItem* newOneItem() const;//获取新建立的图形项
    const double PI = 3.1415926;
    void setPen(QPen p);
protected:
    enum ItemMode {
        Rect,
        ReverseRect,
        Circle,
        Ellipse,
        Ring,        //圆环，即画两个圆
        Arc,         //圆弧
        Ployline        //多边形
    };
    int itemType = 0;//绘制的图形，默认矩形
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    QPointF twoPointSetCircle(QPointF midPoint, QPointF b, qreal& radius);//两点确定一个圆,输入一个圆心与一个点，返回圆心
    QPointF threePointSetCircle(QPointF pt1, QPointF pt2, QPointF pt3, double& radius);//三点确定一个圆，返回圆心
    //通用
    void mPressEventPoint(QGraphicsSceneMouseEvent* event);//点击标记一个点，并加入pressPoint, pointItemLists
    QPointF mEventMovingPoint(QGraphicsSceneMouseEvent* event);//标记正在移动的点，构建图形，不加入两List
    void clearPoint();//清空标记的点 pointItemLists, 即PointItem
    //画圆
    void mPressEventCircle(QGraphicsSceneMouseEvent* event);
    void mMoveEventCircle(QGraphicsSceneMouseEvent* event);
    //画圆环，当做两个圆画，需要不断粘合交互两个圆，不具有整体性，但也可通过交给主程序的item划分圆环范围
    void mPressEventRing(QGraphicsSceneMouseEvent* event);
    void mMoveEventRing(QGraphicsSceneMouseEvent* event);
    //画圆弧，
    void mPressEventArc(QGraphicsSceneMouseEvent* event);
    void mMoveEventArc(QGraphicsSceneMouseEvent* event);
    //画多边形
    void mPressEventPloyline(QGraphicsSceneMouseEvent* event);
    void mMoveEventPloyline(QGraphicsSceneMouseEvent* event);
private:
    QPen pen;//画笔
    //记录鼠标点击
    QVector<QPointF> pressPoint;//记录press的位置
    QVector<PointItem*> pointItemList;//记录press创建的PointItem
    PointItem* mSItem = nullptr;//更新为最新标记的点Point
    //
    QVector<QGraphicsItem*> graphicsItems;//用于多边形存储线
    QGraphicsItem* gItem = nullptr;//场景中唯一的图片图形项
    QGraphicsItem* rItem = nullptr;//场景中通过标记点位创建的图形项
    QGraphicsItem* RItem = nullptr;//作为记录场景画圆环的父亲环，这里不可用图形项组，会融合为一个图形项，需重写事件变更大小
    QPointF pointF;
    bool newItem = false;
    //圆、圆环、圆弧
    qreal R = 0;
    qreal r = 0;
    QPointF midPoint;
    qreal startAngle;
    qreal spanAngle;
    QLineF linesF[3];
    const qreal minGap = 1;//圆环，圆弧，内外半径最小变化(未启用)
    
signals:
    void newItemAchieve(QGraphicsItem*);
};

