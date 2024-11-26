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

    void SnedData(void);

    void CheckData(QByteArray Data);

    void on_btn_send_2_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *time;
    QMessageBox Msg;
    QFile file;
    QByteArray binfile;
    quint8 temp[2];
    int count_port=0;
    uint16_t index=0;
    int len=0;
    quint8 header1=0;
    bool flag_send=false;
    quint8 buff_recive[20];
    int count=0;
    quint32 chsum=0;

    quint8 header2 = 0;
    bool flag_headers = false;

    QByteArray data;
    quint16 counter_data;
};
#endif // MAINWINDOW_H
