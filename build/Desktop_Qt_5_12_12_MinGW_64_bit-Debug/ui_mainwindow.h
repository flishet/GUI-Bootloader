/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btn_open;
    QGroupBox *groupBox_2;
    QComboBox *combo_port;
    QPushButton *btn_port;
    QPushButton *pushButton_2;
    QPushButton *btn_send;
    QLabel *label;
    QProgressBar *progressBar;
    QPushButton *btn_send_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(513, 379);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        btn_open = new QPushButton(centralwidget);
        btn_open->setObjectName(QString::fromUtf8("btn_open"));
        btn_open->setGeometry(QRect(140, 50, 91, 21));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 111, 131));
        combo_port = new QComboBox(groupBox_2);
        combo_port->setObjectName(QString::fromUtf8("combo_port"));
        combo_port->setGeometry(QRect(20, 30, 69, 20));
        btn_port = new QPushButton(groupBox_2);
        btn_port->setObjectName(QString::fromUtf8("btn_port"));
        btn_port->setGeometry(QRect(20, 100, 71, 23));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_port->sizePolicy().hasHeightForWidth());
        btn_port->setSizePolicy(sizePolicy);
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 70, 71, 23));
        btn_send = new QPushButton(centralwidget);
        btn_send->setObjectName(QString::fromUtf8("btn_send"));
        btn_send->setGeometry(QRect(140, 80, 91, 21));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(140, 20, 121, 16));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(140, 110, 118, 23));
        progressBar->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignmentFlag::AlignCenter);
        progressBar->setTextVisible(true);
        btn_send_2 = new QPushButton(centralwidget);
        btn_send_2->setObjectName(QString::fromUtf8("btn_send_2"));
        btn_send_2->setGeometry(QRect(300, 80, 91, 21));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        btn_open->setText(QApplication::translate("MainWindow", "Open", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Port Setting", nullptr));
        btn_port->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        pushButton_2->setText(QApplication::translate("MainWindow", "Refresh Port", nullptr));
        btn_send->setText(QApplication::translate("MainWindow", "Send", nullptr));
        label->setText(QApplication::translate("MainWindow", "TextLabel", nullptr));
        btn_send_2->setText(QApplication::translate("MainWindow", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
