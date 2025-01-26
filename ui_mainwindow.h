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
    QProgressBar *progressBar;
    QLabel *label_2;
    QPushButton *btn_open;
    QPushButton *btn_boot;
    QPushButton *btn_app;
    QLabel *label;
    QComboBox *combo_mcu;
    QPushButton *btn_program;
    QLabel *label_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(525, 220);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(525, 220));
        MainWindow->setMaximumSize(QSize(525, 220));
        MainWindow->setWindowTitle(QString::fromUtf8("GUI Bootloader"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(20, 150, 481, 23));
        progressBar->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        progressBar->setTextVisible(true);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 110, 491, 31));
        label_2->setAlignment(Qt::AlignCenter);
        btn_open = new QPushButton(centralwidget);
        btn_open->setObjectName(QString::fromUtf8("btn_open"));
        btn_open->setGeometry(QRect(110, 80, 91, 21));
        btn_boot = new QPushButton(centralwidget);
        btn_boot->setObjectName(QString::fromUtf8("btn_boot"));
        btn_boot->setGeometry(QRect(410, 80, 91, 21));
        btn_app = new QPushButton(centralwidget);
        btn_app->setObjectName(QString::fromUtf8("btn_app"));
        btn_app->setGeometry(QRect(310, 80, 91, 21));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 481, 41));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        combo_mcu = new QComboBox(centralwidget);
        combo_mcu->setObjectName(QString::fromUtf8("combo_mcu"));
        combo_mcu->setGeometry(QRect(40, 80, 61, 20));
        btn_program = new QPushButton(centralwidget);
        btn_program->setObjectName(QString::fromUtf8("btn_program"));
        btn_program->setGeometry(QRect(210, 80, 91, 21));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 31, 21));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        label_2->setText(QString());
        btn_open->setText(QApplication::translate("MainWindow", "Open", nullptr));
        btn_boot->setText(QApplication::translate("MainWindow", "Run Bootloader", nullptr));
        btn_app->setText(QApplication::translate("MainWindow", "Run Aplication", nullptr));
        label->setText(QString());
        btn_program->setText(QApplication::translate("MainWindow", "Program", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "MCU:", nullptr));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
