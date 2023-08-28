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
    void setNewItem();//��ͨ�������������ͼ���true
    QGraphicsItem* newOneItem() const;//��ȡ�½�����ͼ����
    const double PI = 3.1415926;
    void setPen(QPen p);
protected:
    enum ItemMode {
        Rect,
        ReverseRect,
        Circle,
        Ellipse,
        Ring,        //Բ������������Բ
        Arc,         //Բ��
        Ployline        //�����
    };
    int itemType = 0;//���Ƶ�ͼ�Σ�Ĭ�Ͼ���
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    QPointF twoPointSetCircle(QPointF midPoint, QPointF b, qreal& radius);//����ȷ��һ��Բ,����һ��Բ����һ���㣬����Բ��
    QPointF threePointSetCircle(QPointF pt1, QPointF pt2, QPointF pt3, double& radius);//����ȷ��һ��Բ������Բ��
    //ͨ��
    void mPressEventPoint(QGraphicsSceneMouseEvent* event);//������һ���㣬������pressPoint, pointItemLists
    QPointF mEventMovingPoint(QGraphicsSceneMouseEvent* event);//��������ƶ��ĵ㣬����ͼ�Σ���������List
    void clearPoint();//��ձ�ǵĵ� pointItemLists, ��PointItem
    //��Բ
    void mPressEventCircle(QGraphicsSceneMouseEvent* event);
    void mMoveEventCircle(QGraphicsSceneMouseEvent* event);
    //��Բ������������Բ������Ҫ����ճ�Ͻ�������Բ�������������ԣ���Ҳ��ͨ�������������item����Բ����Χ
    void mPressEventRing(QGraphicsSceneMouseEvent* event);
    void mMoveEventRing(QGraphicsSceneMouseEvent* event);
    //��Բ����
    void mPressEventArc(QGraphicsSceneMouseEvent* event);
    void mMoveEventArc(QGraphicsSceneMouseEvent* event);
    //�������
    void mPressEventPloyline(QGraphicsSceneMouseEvent* event);
    void mMoveEventPloyline(QGraphicsSceneMouseEvent* event);
private:
    QPen pen;//����
    //��¼�����
    QVector<QPointF> pressPoint;//��¼press��λ��
    QVector<PointItem*> pointItemList;//��¼press������PointItem
    PointItem* mSItem = nullptr;//����Ϊ���±�ǵĵ�Point
    //
    QVector<QGraphicsItem*> graphicsItems;//���ڶ���δ洢��
    QGraphicsItem* gItem = nullptr;//������Ψһ��ͼƬͼ����
    QGraphicsItem* rItem = nullptr;//������ͨ����ǵ�λ������ͼ����
    QGraphicsItem* RItem = nullptr;//��Ϊ��¼������Բ���ĸ��׻������ﲻ����ͼ�����飬���ں�Ϊһ��ͼ�������д�¼������С
    QPointF pointF;
    bool newItem = false;
    //Բ��Բ����Բ��
    qreal R = 0;
    qreal r = 0;
    QPointF midPoint;
    qreal startAngle;
    qreal spanAngle;
    QLineF linesF[3];
    const qreal minGap = 1;//Բ����Բ��������뾶��С�仯(δ����)
    
signals:
    void newItemAchieve(QGraphicsItem*);
};

