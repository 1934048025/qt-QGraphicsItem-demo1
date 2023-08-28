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

    //视图框架的构建
    scene = new MyScene;
	graphic = new GraphicsItem;
    scene->addItem(graphic);
    scene->setGraphicsItem(graphic);
    scene->setPen(pen);
    //将ui上显示的view换成可重写的myview
    view = new MyView;
    view->setGeometry(ui.graphicsView->geometry());//(20, 20, 981, 671)
    view->setParent(ui.groupBox);
    view->setScene(scene);
    //QColor换成Qt::gray会覆盖背景与图形
    scene->setBackgroundBrush(QColor(70, 70, 70, 200));
    //设置QCommoBox的菜单
    connect(ui.comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QtDemo1::itemChange);//6
    //获取场景通过鼠标移动建立的图形项，方便菜单栏更换后进行移除
     connect(scene, &MyScene::newItemAchieve, this, &QtDemo1::getNewItem);
    itemChange(0);//初始化为矩形
}

QtDemo1::~QtDemo1()
{}

void QtDemo1::itemChange(int index)
{
    if (item != nullptr) {
        if (dynamic_cast<CircleItem*>(item->parentItem()) != nullptr)
            scene->removeItem(item->parentItem());//圆环
        else
            scene->removeItem(item);
        //delete item;
        item = nullptr;
    }
	switch (ui.comboBox->currentIndex())
	{
	case 0: {//矩形
		item = new RectItem;
		break;
	}
    case 1: {//旋转矩形
        item = new ReverseRectItem;
        break;
	}
    case 2: {//圆形
        scene->setNewItem();
        scene->setItemType(index);//由于新图形项目前为空则，单独更改，启动变更图形
		break;
	}
    case 3: {//椭圆
        item = new EllipseItem;
        break;
    }
    case 4: {//圆环
        scene->setNewItem();
        scene->setItemType(index);
        break;
    }
    case 5: {//圆弧
        scene->setNewItem();
        scene->setItemType(index);
        break;
    }
    case 6: {//多边形
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