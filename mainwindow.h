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
#include <QThread>
#include <chrono>
#include <thread>
#include <QUdpSocket>





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

    void on_btn_program_clicked();

    void ReadyReads(void);

    void IntervalTimer(void);

    void SnedData(void);

    void CheckData(unsigned char Data);

    void on_btn_boot_clicked();

    uint16_t CRC16_Modbus(QByteArray data, quint16 length);

    void on_btn_app_clicked();

    void on_btn_convert_clicked();

    void processPendingDatagrams();

    void on_rd_lan_clicked(bool checked);

    void on_rd_serial_clicked(bool checked);

    void on_btn_listen_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *time;
    QMessageBox Msg;
    QFile file,out;
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
    bool flag_write=false;
    bool flag_ok=false;
    QByteArray data;
    quint16 counter_data;
    bool flag_file_valid=false;
    QUdpSocket *udpSocket;
    quint16 timeout=0;



    void sendLength(quint8 cmd);
    void AckRecive(QByteArray cmd);
};
#endif // MAINWINDOW_H
