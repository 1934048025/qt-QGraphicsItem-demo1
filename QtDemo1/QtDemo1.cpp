#include "QtDemo1.h"
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QColor>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QComboBox>
#include "MyView.h"
#include "RectItem.h"
#include "ReverseRectItem.h"
#include "EllipseItem.h"
#include "CircleItem.h"
#include "MyScene.h"

QtDemo1::QtDemo1(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    //��ͼ��ܵĹ���
    scene = new MyScene;
	graphic = new GraphicsItem;
    scene->addItem(graphic);
    scene->setGraphicsItem(graphic);
    scene->setPen(pen);
    //��ui����ʾ��view���ɿ���д��myview
    view = new MyView;
    view->setGeometry(ui.graphicsView->geometry());//(20, 20, 981, 671)
    view->setParent(ui.groupBox);
    view->setScene(scene);
    //QColor����Qt::gray�Ḳ�Ǳ�����ͼ��
    scene->setBackgroundBrush(QColor(70, 70, 70, 200));
    //����QCommoBox�Ĳ˵�
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QtDemo1::itemChange);//6
    //��ȡ����ͨ������ƶ�������ͼ�������˵�������������Ƴ�
     connect(scene, &MyScene::newItemAchieve, this, &QtDemo1::getNewItem);
    itemChange(0);//��ʼ��Ϊ����
}

QtDemo1::~QtDemo1()
{}

void QtDemo1::itemChange(int index)
{
    if (item != nullptr) {
        if (dynamic_cast<CircleItem*>(item->parentItem()) != nullptr)
            scene->removeItem(item->parentItem());//Բ��
        else
            scene->removeItem(item);
        //delete item;
        item = nullptr;
    }
	switch (ui.comboBox->currentIndex())
	{
	case 0: {//����
		item = new RectItem;
		break;
	}
    case 1: {//��ת����
        item = new ReverseRectItem;
        break;
	}
    case 2: {//Բ��
        scene->setNewItem();
        scene->setItemType(index);//������ͼ����ĿǰΪ���򣬵������ģ��������ͼ��
		break;
	}
    case 3: {//��Բ
        item = new EllipseItem;
        break;
    }
    case 4: {//Բ��
        scene->setNewItem();
        scene->setItemType(index);
        break;
    }
    case 5: {//Բ��
        scene->setNewItem();
        scene->setItemType(index);
        break;
    }
    case 6: {//�����
        scene->setNewItem();
        scene->setItemType(index);
        break;
    }
    
	default:
		break;
	}
    if (item) {
		item->setParentItem(graphic);
        scene->setItemType(index);
    }
    view->update();
}

void QtDemo1::getNewItem(QGraphicsItem *nItem)
{
    item = nItem;
    item->setParentItem(graphic);
}