#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QFileDialog>
#include <QLabel>
#include <qdebug.h>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btn_open_clicked();

    void on_btn_port_clicked();

    void on_pushButton_2_clicked();

    void on_btn_send_clicked();

    void ReadyReads(void);

    void IntervalTimer(void);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *time;
    QMessageBox Msg;
    QFile file;
    QByteArray binfile;

    int count_port=0;
    uint16_t index=0;

};
#endif // MAINWINDOW_H
