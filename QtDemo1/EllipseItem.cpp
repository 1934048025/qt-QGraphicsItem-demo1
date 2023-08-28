#include "EllipseItem.h"
#include <QPainter>

EllipseItem::EllipseItem()
{
	this->setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF EllipseItem::boundingRect() const
{
	qreal adjust = 0.5;
	return ReverseRectItem::boundingRect();
}

void EllipseItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	//清除
	while (itemList.size() > 0)
	{
		pItem = itemList[0];
		itemList.erase(itemList.begin());
		parentItem()->scene()->removeItem(pItem);
		pItem = nullptr;
	}
	//更新标点
	UpdateHandlesArea();
	for (int i = 0; i < mouseNum - 2; i++) {
		pItem = new PointItem(m_handlesArea[i]);
		pItem->setParentItem(this);
		itemList.push_back(pItem);
		pItem = nullptr;
	}
	painter->setPen(pen);
	painter->drawEllipse(m_rect);
	if (this->isSelected()) {
		//绘制旋转点
		painter->setPen(penCir);
		painter->setBrush(penCir.color());
		painter->drawLine(m_rect.center().x(), m_rect.top(), m_rect.center().x(), m_rect.top() - gap);
		painter->drawEllipse(m_rect.center().x() - reverCircleR, m_rect.top() - gap, 2 * reverCircleR, 2 * reverCircleR);
	}
}
