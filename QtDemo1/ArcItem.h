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
    //ֻ������
    ArcItem(const QRectF& rectF, const qreal& startAngle, const qreal& spanAngle, const bool &pointFlag = true);
    //��Բ��
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
        mRaLeft,    //��Բ�� startAngle + spanAngle
        mRaRight,   //��Բ�� startAngle
        mRaMiddle,  //��Բ����
        raLeft, //��Բ�� startAngle + spanAngle
        raRight,    //��Բ�� startAngle
        raMiddle,  //��Բ����
        mReverseMiddle, //��ת��
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
	QPen pen = QPen(Qt::blue, 2);//����
    //�߽���Σ�Բ�ģ� ��ʼ�Ƕȣ���ת�Ƕ�
    QRectF rect;
    QPointF midPoint;
    qreal startAngle;//��*16
    qreal spanAngle;
    qreal radius;//��Բ�뾶
    qreal Radius;//��Բ�뾶
    bool pointFlag = true;//�Ƿ���Ҫ��Ǳ��
    bool arcsFlag = true;//�Ƿ���Բ���������߲��ر�ǹ̶��㣬������
    const qreal PI = 3.1415926;
    //��ǵ�
    QRectF mouseHandles[8];//�̶���8�ı�ǵ㷶Χ
    int mHandleNum = 8;//����ǵ���̬����
    qreal mHandlePointSize = 6;//��ǵ���
	//��ǵ�Ļ���
	PointItem* pItem = nullptr;
	QVector<PointItem*> itemList;
    //  ��ת
	qreal reverCircleR = 3;//��תԲ�İ뾶
	qreal gap = 16;//��תԲ�����εľ���
	qreal moreSize = reverCircleR + gap;//ϸ�ڴ���bounding�ı仯
	QPen penCir = QPen(Qt::red, 2);//��תԲ��
	QRectF boundRect;//�������m_rect������Ƕ���ͨ��moreSize���б߽��������ӡ�
	QPointF pressPoint;//��¼��갴�µĵ�,��ת

};

