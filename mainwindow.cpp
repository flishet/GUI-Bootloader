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
    ui->progressBar->setVisible(false);
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


void MainWindow::SnedData(void)
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
        all2byte.all=index;
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        temp.append(binfile.length()%16);

        for(int i=0;i<binfile.length()%16;i++)
        {
            sum+=(quint8)binfile.at(index*16+i);
            temp.append(binfile.at(index*16+i));
        }
        all2byte.all=sum;
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        // qDebug()<<index;
        serial->write(temp,temp.length());
        index++;
        time->stop();
        ui->progressBar->setValue((100/(float)len)*index);
        // Msg.information(nullptr,"information","send ok",QMessageBox::Ok);
        ui->progressBar->setVisible(false);
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
        // qDebug()<<index;
        index++;
        serial->write(temp,temp.length());
        ui->progressBar->setValue((100/(float)len)*index);
    }
}

void MainWindow::CheckData(QByteArray Data)
{
    //    union {
    //        uint8_t byte[2];
    //        uint16_t all;
    //    }all2byte;

    //    int count_rec=0;
    //    quint8 temp[20];

    // qDebug()<< Data.length()<< ++packet_count;
    for (int i = 0; i < Data.length(); ++i)
    {
        header2 = header1;
        header1 =(quint8) Data[i];

        if (header2 == 0xC1 && header1 == 0xB7 && flag_headers==false)
        {
            flag_headers = true;
        }
        else if(flag_headers == true)
        {
            if(counter_data == 0)
            {
                data[counter_data] = Data[i];
                counter_data++;
            }
            else if(counter_data == 1)
            {
                data[counter_data] = Data[i];
                counter_data++;
            }
            else if(counter_data == 2)
            {
                data[counter_data] = Data[i];
                counter_data++;
            }
            else if(counter_data == 3)
            {
                data[counter_data] = Data[i];
                counter_data++;
            }

            if(counter_data == 4)
            {
                quint16 sum = (quint8)data[0] + (quint8)data[1];

                quint16 check_sum = (quint8)data[2] + (quint8)data[3] * 256;

                qDebug()<< __LINE__<< counter_data << sum << check_sum;

                if( sum == check_sum )
                {
                    quint16 value = 0;
                    memcpy(&value, data.data(), 2);


                    if( value + 1 == index ){
                        flag_headers=false;
                        flag_send=true;

                        qDebug()<< __LINE__<< ", value:" << (quint16)value << Data.toHex(' ') << ", index:" << index;
                    }

                }

                counter_data = 0;
                data.clear();
            }


            //            temp[count_rec]=(quint8)Data[i];
            //            //            qDebug()<<hex<<temp[0]<<count_rec<<Data.toHex(' ');
            //            //            if(count_rec==6)
            //            {
            //                quint16 value = 0;
            //                memcpy(&value, Data.data() + i, 2);

            //                qDebug()<< __LINE__<< ", value:" << (quint16)value << Data.toHex(' ') << ", index:" << index;
            //                //                qDebug()<< "value:" << value << ", index:" << index;

            //                if ( value + 1 == index ){


            //                    flag_headers=false;
            //                    flag_send=true;
            //                    break;
            //                }
            //                //                all2byte.byte[0]=temp[4];
            //                //                all2byte.byte[1]=temp[5];
            //                //                if(all2byte.all==(temp[2]+temp[3]))
            //                //                {
            //                //                    all2byte.byte[0]=temp[2];
            //                //                    all2byte.byte[1]=temp[3];
            //                //                    if((all2byte.all+1)==index)
            //                //                    {
            //                //                        flag_headers=false;
            //                //                        flag_send=true;
            //                //                        qDebug()<< __LINE__;
            //                //                    }
            //                //                }
            //            }
            //            //            else
            //            //                count_rec++;
        }
    }
}

void MainWindow::IntervalTimer(void)
{
    if(flag_send)
    {
        flag_send=false;
        SnedData();
    }
}

void MainWindow::ReadyReads(void)
{
    QByteArray data;
    data=serial->readAll();
    // for(int i=0;i<data.length();i++)
    CheckData(data);
}

void MainWindow::on_btn_open_clicked()
{
    QString filePath ="C:/Users/RayanRoshd/Desktop/mohsen/b.bin"; //QFileDialog::getOpenFileName(this, "Open Binary File", "", "BIN (*.bin)");
    if (!filePath.isEmpty())
    {
        file.setFileName(filePath);
        file.open(QIODevice::ReadOnly);
        binfile=file.readAll();
        len=file.size()/16;
        if((file.size()%16)!=0)
            len++;
        ui->label->setText(QString::number(file.size(),10)+" Byte"+" Len="+QString::number(len,10));
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

    ui->progressBar->setVisible(true);
    time->start(20);
    index=0;
    SnedData();
    // flag_send=true;
}



void MainWindow::on_btn_send_2_clicked()
{
    SnedData();
}

