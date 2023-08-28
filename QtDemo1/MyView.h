#pragma once
#include <QGraphicsView>
class MyView :
    public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyView(QWidget* parent = 0);
protected:
    void wheelEvent(QWheelEvent* event);
};

