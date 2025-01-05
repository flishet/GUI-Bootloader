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
#include <QShortcut>
#include <QKeySequence>




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+k"), this);


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

    void on_combo_mcu_currentTextChanged(const QString &arg1);

    void toggleWidgets(void);

    void on_btn_port_2_clicked();

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
    quint32 DeviceID=0;
    quint32 SelectDevice=0;
    quint8 header2 = 0;
    int timeout2=0;
    bool flag_headers = false;
    bool flag_timeout=false;
    bool flag_write=false;
    bool flag_ok=false;
    QByteArray data;
    quint16 counter_data;
    bool flag_file_valid=false;
    bool timeout_erase=false;
    quint8 index_erase=0;
    QUdpSocket *udpSocket;
    quint16 timeout=0;

    quint32 val1=0,val2=0;
    quint8 val3=0,val4=0;
    union {
        uint8_t byte[2];
        uint16_t all;
    }all2byte;

bool flag_tms=false;
    void sendLength(quint8 cmd);
    void AckRecive(QByteArray cmd);
};
#endif // MAINWINDOW_H
