/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *inputLayout;
    QLabel *labelN;
    QLineEdit *editN;
    QLabel *labelM;
    QLineEdit *editM;
    QPushButton *btnInit;
    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout;
    QLabel *labelSpots;
    QListWidget *listParking;
    QVBoxLayout *rightLayout;
    QLabel *labelQueue;
    QListWidget *listQueue;
    QHBoxLayout *bottomLayout;
    QLabel *labelCarNum;
    QLineEdit *editCarNum;
    QPushButton *btnRandPlate;
    QPushButton *btnEnter;
    QPushButton *btnExit;
    QTextEdit *textFee;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(650, 500);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        inputLayout = new QHBoxLayout();
        inputLayout->setObjectName("inputLayout");
        labelN = new QLabel(centralwidget);
        labelN->setObjectName("labelN");

        inputLayout->addWidget(labelN);

        editN = new QLineEdit(centralwidget);
        editN->setObjectName("editN");

        inputLayout->addWidget(editN);

        labelM = new QLabel(centralwidget);
        labelM->setObjectName("labelM");

        inputLayout->addWidget(labelM);

        editM = new QLineEdit(centralwidget);
        editM->setObjectName("editM");

        inputLayout->addWidget(editM);

        btnInit = new QPushButton(centralwidget);
        btnInit->setObjectName("btnInit");

        inputLayout->addWidget(btnInit);


        verticalLayout->addLayout(inputLayout);

        mainLayout = new QHBoxLayout();
        mainLayout->setObjectName("mainLayout");
        leftLayout = new QVBoxLayout();
        leftLayout->setObjectName("leftLayout");
        labelSpots = new QLabel(centralwidget);
        labelSpots->setObjectName("labelSpots");

        leftLayout->addWidget(labelSpots);

        listParking = new QListWidget(centralwidget);
        listParking->setObjectName("listParking");

        leftLayout->addWidget(listParking);


        mainLayout->addLayout(leftLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setObjectName("rightLayout");
        labelQueue = new QLabel(centralwidget);
        labelQueue->setObjectName("labelQueue");

        rightLayout->addWidget(labelQueue);

        listQueue = new QListWidget(centralwidget);
        listQueue->setObjectName("listQueue");

        rightLayout->addWidget(listQueue);


        mainLayout->addLayout(rightLayout);


        verticalLayout->addLayout(mainLayout);

        bottomLayout = new QHBoxLayout();
        bottomLayout->setObjectName("bottomLayout");
        labelCarNum = new QLabel(centralwidget);
        labelCarNum->setObjectName("labelCarNum");

        bottomLayout->addWidget(labelCarNum);

        editCarNum = new QLineEdit(centralwidget);
        editCarNum->setObjectName("editCarNum");

        bottomLayout->addWidget(editCarNum);

        btnRandPlate = new QPushButton(centralwidget);
        btnRandPlate->setObjectName("btnRandPlate");

        bottomLayout->addWidget(btnRandPlate);

        btnEnter = new QPushButton(centralwidget);
        btnEnter->setObjectName("btnEnter");

        bottomLayout->addWidget(btnEnter);

        btnExit = new QPushButton(centralwidget);
        btnExit->setObjectName("btnExit");

        bottomLayout->addWidget(btnExit);


        verticalLayout->addLayout(bottomLayout);

        textFee = new QTextEdit(centralwidget);
        textFee->setObjectName("textFee");

        verticalLayout->addWidget(textFee);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\201\234\350\275\246\345\234\272\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        labelN->setText(QCoreApplication::translate("MainWindow", "\350\275\246\344\275\215\346\225\260 n\357\274\232", nullptr));
        labelM->setText(QCoreApplication::translate("MainWindow", "\351\230\237\345\210\227\344\270\212\351\231\220 m\357\274\232", nullptr));
        btnInit->setText(QCoreApplication::translate("MainWindow", "\345\210\235\345\247\213\345\214\226\345\201\234\350\275\246\345\234\272", nullptr));
        labelSpots->setText(QCoreApplication::translate("MainWindow", "\347\251\272\344\275\231\350\275\246\344\275\215\357\274\232", nullptr));
        labelQueue->setText(QCoreApplication::translate("MainWindow", "\346\216\222\351\230\237\350\275\246\350\276\206\346\225\260\357\274\232", nullptr));
        labelCarNum->setText(QCoreApplication::translate("MainWindow", "\350\275\246\347\211\214\345\217\267\357\274\232", nullptr));
        btnRandPlate->setText(QCoreApplication::translate("MainWindow", "\351\232\217\346\234\272\350\275\246\347\211\214", nullptr));
        btnEnter->setText(QCoreApplication::translate("MainWindow", "\350\275\246\350\276\206\345\205\245\345\234\272", nullptr));
        btnExit->setText(QCoreApplication::translate("MainWindow", "\350\275\246\350\276\206\345\207\272\345\234\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
