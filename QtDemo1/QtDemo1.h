#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtDemo1.h"
#include <QGraphicsScene>
#include "GraphicsItem.h"
#include <QGraphicsView>
#include "MyView.h"
#include "MyScene.h"

class QtDemo1 : public QWidget
{
    Q_OBJECT

public:
    QtDemo1(QWidget *parent = nullptr);
    ~QtDemo1();
    QPen pen = QPen(Qt::blue, 2);
public slots:
    void itemChange(int);
    void getNewItem(QGraphicsItem*);

private:
    Ui::QtDemo1Class ui;
    MyView* view = nullptr;
    MyScene* scene = nullptr;
    GraphicsItem* graphic = nullptr;
    QGraphicsItem* item = nullptr;
};
