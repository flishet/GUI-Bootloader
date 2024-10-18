#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    lbl = new QLabel(this);

    ui->setupUi(this);
    ui->format_mono->setChecked(true);
    ui->scrollArea->setWidget(lbl);

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
    int sum=0;
    union {
        uint8_t byte[2];
        uint16_t all;
    }all2byte;

    temp.append(0xff);
    temp.append(0xcc);
    temp.append(0x05);

    if(index>=data.length()/16)
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
            sum+=(quint8)data.at(index*16+i);
            temp.append(data.at(index*16+i));
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

    QString filePath = QFileDialog::getOpenFileName(this, "Open Image File", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        image.load(filePath,NULL);
        if (!image.isNull())
        {
            lbl->setPixmap(QPixmap::fromImage(image));
            lbl->adjustSize();
            convert_print(image,&data);
            qDebug()<<hex<<data<<data.size();
        }
    }
}


void MainWindow::on_btn_convert_clicked()
{
    QImage imageColor(256, 128, QImage::Format_RGB444);
    imageColor.fill(Qt::white);

    QPainter painter(&imageColor);

    QFont font("Arial", 30);
    painter.setFont(font);
    painter.setPen(Qt::black);

    QString text = "Hello, Qt!";
    QRect rect = imageColor.rect();
    painter.drawText(rect, Qt::AlignCenter, text);

    painter.end();

    QImage imageMono;
    if(ui->format_16color->isChecked())
        imageMono=imageColor.convertToFormat(QImage::Format_Indexed8);
    else
        imageMono=imageColor.convertToFormat(QImage::Format_Mono);

    QPixmap pixmap = QPixmap::fromImage(imageMono);
    lbl->scroll(1200,0);
    lbl->setPixmap(pixmap);
    lbl->resize(pixmap.size());
    lbl->adjustSize();
    imageMono.save("text_image_mono.bmp", "BMP");
}

void MainWindow::convert_print(QImage image, QByteArray *data)
{
    int h=0;
    uint8_t temp=0;
    for(int i=0;i<image.width();i++)
    {
        for(int j=0;j<image.height();j++)
        {
            temp|=(image.pixelIndex(i,j)<<h);
            if(++h>=8)
            {
                h=0;
                //qDebug()<<hex<<temp;
                data->append(temp);
                temp=0;
            }
        }
    }
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


