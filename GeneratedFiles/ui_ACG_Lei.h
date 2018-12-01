/********************************************************************************
** Form generated from reading UI file 'ACG_Lei.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACG_LEI_H
#define UI_ACG_LEI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ACG_LeiClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionLoop;
    QAction *actionButterfly;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuSubdivision;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ACG_LeiClass)
    {
        if (ACG_LeiClass->objectName().isEmpty())
            ACG_LeiClass->setObjectName(QString::fromUtf8("ACG_LeiClass"));
        ACG_LeiClass->resize(728, 485);
        actionOpen = new QAction(ACG_LeiClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSave = new QAction(ACG_LeiClass);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionLoop = new QAction(ACG_LeiClass);
        actionLoop->setObjectName(QString::fromUtf8("actionLoop"));
        actionButterfly = new QAction(ACG_LeiClass);
        actionButterfly->setObjectName(QString::fromUtf8("actionButterfly"));
        centralWidget = new QWidget(ACG_LeiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ACG_LeiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ACG_LeiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 728, 26));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSubdivision = new QMenu(menuBar);
        menuSubdivision->setObjectName(QString::fromUtf8("menuSubdivision"));
        ACG_LeiClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ACG_LeiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ACG_LeiClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuSubdivision->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuSubdivision->addAction(actionLoop);
        menuSubdivision->addAction(actionButterfly);

        retranslateUi(ACG_LeiClass);
        QObject::connect(actionOpen, SIGNAL(triggered()), ACG_LeiClass, SLOT(action_open()));
        QObject::connect(actionSave, SIGNAL(triggered()), ACG_LeiClass, SLOT(action_save()));
        QObject::connect(actionLoop, SIGNAL(triggered()), ACG_LeiClass, SLOT(action_loop()));
        QObject::connect(actionButterfly, SIGNAL(triggered()), ACG_LeiClass, SLOT(action_butterfly()));

        QMetaObject::connectSlotsByName(ACG_LeiClass);
    } // setupUi

    void retranslateUi(QMainWindow *ACG_LeiClass)
    {
        ACG_LeiClass->setWindowTitle(QApplication::translate("ACG_LeiClass", "ACG_Lei", nullptr));
        actionOpen->setText(QApplication::translate("ACG_LeiClass", "Open", nullptr));
        actionSave->setText(QApplication::translate("ACG_LeiClass", "Save", nullptr));
        actionLoop->setText(QApplication::translate("ACG_LeiClass", "Loop", nullptr));
        actionButterfly->setText(QApplication::translate("ACG_LeiClass", "Butterfly", nullptr));
        menuFile->setTitle(QApplication::translate("ACG_LeiClass", "File", nullptr));
        menuSubdivision->setTitle(QApplication::translate("ACG_LeiClass", "Subdivision", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ACG_LeiClass: public Ui_ACG_LeiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACG_LEI_H
