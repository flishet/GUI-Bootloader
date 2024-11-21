#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    time = new QTimer(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(ReadyReads()));
    connect(time,SIGNAL(timeout()),this,SLOT(IntervalTimer()));
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->combo_port->addItem(port.portName());
        serial->setPortName (port.portName());
        serial->close ();
        count_port++;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::IntervalTimer(void)
{
    QByteArray temp;
    quint16 sum=0;
    union {
        uint8_t byte[2];
        uint16_t all;
    }all2byte;

    temp.append(0xc1);
    temp.append(0xb7);
    temp.append(0x15);

    if(index>=binfile.length()/16)
    {
        time->stop();
    }
    else
    {
        all2byte.all=index;
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        temp.append(16);

        for(int i=0;i<16;i++)
        {
            sum+=(quint8)binfile.at(index*16+i);
            temp.append(binfile.at(index*16+i));
        }
        all2byte.all=sum;
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        qDebug()<<hex<<sum<<index;
        index++;
        serial->write(temp,temp.length());
    }
}

void MainWindow::ReadyReads(void)
{
//    static int i=0;
//    ui->btn_send->setText(QString::number(i++,10));
}

void MainWindow::on_btn_open_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Binary File", "", "BIN (*.bin)");
    if (!filePath.isEmpty())
    {
        file.setFileName(filePath);
        file.open(QIODevice::ReadOnly);
        binfile=file.readAll();
        ui->label->setText(QString::number(file.size(),10)+" Byte");
        qDebug()<<binfile<<filePath;
    }
    else
        Msg.warning(nullptr,"اخطار","فایلی انتخاب نشده است",QMessageBox::Ok);
}


void MainWindow::on_btn_port_clicked()
{
    serial->setPortName(ui->combo_port->currentText()); //ListStr[0]
    if(serial->isOpen() == false)
    {
        if(serial->open(QIODevice::ReadWrite))
        {
            ui->combo_port->setEnabled (false);
            ui->btn_port->setText("Disconnect");
        }
        else if(!serial->open(QIODevice::ReadWrite))
        {
            ui->combo_port->setEnabled (true);
            ui->btn_port->setText("Connect");
            Msg.setText("This serial port is used by another device");
            Msg.exec();
        }
    }
    else if(serial->isOpen() == true)
    {
        serial->close();
        ui->combo_port->setEnabled (true);
        ui->btn_port->setText("Connect");
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->combo_port->clear ();
    foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->combo_port->addItem (port.portName());
    }
}


void MainWindow::on_btn_send_clicked()
{
    index=0;
    time->start(20);
}


