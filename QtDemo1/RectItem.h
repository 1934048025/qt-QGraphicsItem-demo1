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
		Mouse_Reverse,//������ת������Բ
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
	

	const QPen pen = QPen(Qt::blue, 2);//����

	const double m_dHandleSize = 6; //�����εķ�Χ
	QRectF m_handlesArea[10];//�����̬
	int mouseNum = 10;

	// ��Ҫ���Ƴ�ʼ�ľ���
	QRectF m_rect = QRectF(20, 20, 300, 200);

	// ���Ʊ�־����ֹ��һ������϶�����ʱ��ͼԪ������ƶ��¼�������ʹ��ͼԪ�ƶ�
	//�������δʹ���϶�ʽ��ͼ
	bool m_bDrawFlag = false;

	//�м����
	QRectF m_pressRect;
	const qreal m_dAdjust = 1;
	const qreal dMixSize = 2.0 * (1 + m_dAdjust);   // ����ʱ����С�ߴ�
	//��ǵ�Ļ���
	PointItem* pItem = nullptr;
	std::vector<PointItem*> itemList;
private:
	
};

