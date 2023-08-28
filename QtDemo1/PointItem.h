#pragma once
#include <qgraphicsitem.h>
#include <QPainter>
class PointItem :
    public QGraphicsItem
{
public:
    PointItem(const QRectF &r);
    PointItem(const QPointF& p, const qreal& x = 6,const qreal& y = 6);
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    qreal mouseHandleSize() const;
private:
    const qreal m_dHandleSize = 6; //鼠标变形的范围,也是标记点的大小
    const QColor m_color = QColor(Qt::green);//标记颜色
    QRectF mRect = QRectF(0, 0, 6, 6);
};

