/********************************************************************************
** Form generated from reading UI file 'MapZone.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPZONE_H
#define UI_MAPZONE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapZone
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *map_layout;

    void setupUi(QWidget *MapZone)
    {
        if (MapZone->objectName().isEmpty())
            MapZone->setObjectName("MapZone");
        MapZone->resize(672, 635);
        verticalLayout = new QVBoxLayout(MapZone);
        verticalLayout->setObjectName("verticalLayout");
        map_layout = new QVBoxLayout();
        map_layout->setObjectName("map_layout");

        verticalLayout->addLayout(map_layout);


        retranslateUi(MapZone);

        QMetaObject::connectSlotsByName(MapZone);
    } // setupUi

    void retranslateUi(QWidget *MapZone)
    {
        MapZone->setWindowTitle(QCoreApplication::translate("MapZone", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapZone: public Ui_MapZone {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPZONE_H
