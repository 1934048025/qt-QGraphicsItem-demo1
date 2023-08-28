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
	const QPen pen = QPen(Qt::blue, 2);//����
	const QPen penMiddle = QPen(Qt::red, 2);//Բ�ĵĻ���
	
	bool pointItemFlag = true;//�ж��Ƿ�ҪΪԲ�����ܻ����ĸ���ǵ�
	const double m_dHandleSize = 6; //�����εķ�Χ
	const QColor m_color = QColor(Qt::green);//�����ɫ
	QRectF m_handlesArea[5];//�����̬
	int mouseNum = 5;//��̬����
	// ��Ҫ���Ƴ�ʼ�ľ���
	QRectF m_rect = QRectF(0, 0, 200, 200);
	qreal middleLon = 4; //m_rect.width() / 50;//Բ���ı곤�ȣ��������Ʋ���׼

	// ���Ʊ�־����ֹ��һ������϶�����ʱ��ͼԪ������ƶ��¼�������ʹ��ͼԪ�ƶ�
	bool m_bDrawFlag = false;//δ����

	//�м����
	QRectF m_pressRect;
	QPointF midPoint;//Բ��
	const qreal m_dAdjust = 1;
	const qreal dMixSize = 2.0 * (1 + m_dAdjust);   // ����ʱ����С�ߴ�
	
	//��ǵ�Ļ���
	PointItem* pItem = nullptr;
	QVector<PointItem*> itemList;
};
