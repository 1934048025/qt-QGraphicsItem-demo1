/********************************************************************************
** Form generated from reading UI file 'QtDemo1.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTDEMO1_H
#define UI_QTDEMO1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtDemo1Class
{
public:
    QGroupBox *groupBox;
    QGraphicsView *graphicsView;
    QComboBox *comboBox;

    void setupUi(QWidget *QtDemo1Class)
    {
        if (QtDemo1Class->objectName().isEmpty())
            QtDemo1Class->setObjectName(QString::fromUtf8("QtDemo1Class"));
        QtDemo1Class->resize(1313, 796);
        groupBox = new QGroupBox(QtDemo1Class);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 90, 1291, 711));
        graphicsView = new QGraphicsView(groupBox);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(20, 20, 981, 671));
        comboBox = new QComboBox(groupBox);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(1020, 30, 141, 31));

        retranslateUi(QtDemo1Class);

        QMetaObject::connectSlotsByName(QtDemo1Class);
    } // setupUi

    void retranslateUi(QWidget *QtDemo1Class)
    {
        QtDemo1Class->setWindowTitle(QCoreApplication::translate("QtDemo1Class", "QtDemo1", nullptr));
        groupBox->setTitle(QCoreApplication::translate("QtDemo1Class", "GroupBox", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("QtDemo1Class", "\347\237\251\345\275\242", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("QtDemo1Class", "\346\227\213\350\275\254\347\237\251\345\275\242", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("QtDemo1Class", "\345\234\206\345\275\242", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("QtDemo1Class", "\346\244\255\345\234\206", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("QtDemo1Class", "\345\234\206\347\216\257", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("QtDemo1Class", "\345\234\206\345\274\247", nullptr));
        comboBox->setItemText(6, QCoreApplication::translate("QtDemo1Class", "\345\244\232\350\276\271\345\275\242", nullptr));

    } // retranslateUi

};

namespace Ui {
    class QtDemo1Class: public Ui_QtDemo1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTDEMO1_H
