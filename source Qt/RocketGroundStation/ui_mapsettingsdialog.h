/********************************************************************************
** Form generated from reading UI file 'mapsettingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPSETTINGSDIALOG_H
#define UI_MAPSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_MapSettingsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *formGroupBox;
    QFormLayout *formLayout;
    QLabel *mapSourceLabel;
    QComboBox *mapSourceBox;
    QLabel *mapLocationLabel;
    QComboBox *mapLocationBox;

    void setupUi(QDialog *MapSettingsDialog)
    {
        if (MapSettingsDialog->objectName().isEmpty())
            MapSettingsDialog->setObjectName(QString::fromUtf8("MapSettingsDialog"));
        MapSettingsDialog->resize(287, 300);
        MapSettingsDialog->setMinimumSize(QSize(287, 300));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/iconeMap/icone/map.png"), QSize(), QIcon::Normal, QIcon::Off);
        MapSettingsDialog->setWindowIcon(icon);
        MapSettingsDialog->setModal(false);
        buttonBox = new QDialogButtonBox(MapSettingsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(120, 260, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formGroupBox = new QGroupBox(MapSettingsDialog);
        formGroupBox->setObjectName(QString::fromUtf8("formGroupBox"));
        formGroupBox->setGeometry(QRect(10, 0, 261, 251));
        formLayout = new QFormLayout(formGroupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        mapSourceLabel = new QLabel(formGroupBox);
        mapSourceLabel->setObjectName(QString::fromUtf8("mapSourceLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, mapSourceLabel);

        mapSourceBox = new QComboBox(formGroupBox);
        mapSourceBox->setObjectName(QString::fromUtf8("mapSourceBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, mapSourceBox);

        mapLocationLabel = new QLabel(formGroupBox);
        mapLocationLabel->setObjectName(QString::fromUtf8("mapLocationLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, mapLocationLabel);

        mapLocationBox = new QComboBox(formGroupBox);
        mapLocationBox->setObjectName(QString::fromUtf8("mapLocationBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, mapLocationBox);


        retranslateUi(MapSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MapSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MapSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(MapSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *MapSettingsDialog)
    {
        MapSettingsDialog->setWindowTitle(QCoreApplication::translate("MapSettingsDialog", "Map Settings", nullptr));
        formGroupBox->setTitle(QCoreApplication::translate("MapSettingsDialog", "Settings", nullptr));
        mapSourceLabel->setText(QCoreApplication::translate("MapSettingsDialog", "Map Source", nullptr));
        mapLocationLabel->setText(QCoreApplication::translate("MapSettingsDialog", "Map Location", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapSettingsDialog: public Ui_MapSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPSETTINGSDIALOG_H
