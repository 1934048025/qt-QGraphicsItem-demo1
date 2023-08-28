#pragma once
#include <qgraphicsitem.h>
#include <QPen>
#include <QCursor>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include "PointItem.h"


class PolygonItem :
    public QGraphicsItem
{
public:
    PolygonItem(QVector<QPointF> points);
    virtual ~PolygonItem();
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget /* = nullptr */);
    void renewRect();
    void setPen(QPen p);
protected:
    enum mHandle {
        pointMouse,
        noMouse
    }mouseHandle;
    void setMouseCursor(QGraphicsSceneHoverEvent* event);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverMoveEvent(QGraphicsSceneHoverEvent* event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
private:
    QRectF rect;
    QVector<QPointF> points;
    QPen pen = QPen(Qt::blue, 2);//����
	//��ǵ�Ļ���
    qreal mPointHandleSize = 6;//��ǵ�Ŀ��
	PointItem* pItem = nullptr;
	QVector<PointItem*> itemList;
    int index;//���ѡ�б����list�е�λ��
};

