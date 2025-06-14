#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    time = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    ui->rd_serial->setChecked(true);
    ui->groupBox_2->setEnabled(true);

    udpSocket->close();
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);

    ui->combo_mcu->addItem("LPC1788");
    ui->combo_mcu->addItem("LPC1768");
    ui->combo_mcu->addItem("STM32H743");
    ui->combo_mcu->addItem("TMS320F28377S");

    ui->combo_baud->addItem("9600");
    ui->combo_baud->addItem("115200");
    ui->combo_baud->addItem("230400");
    ui->combo_baud->addItem("460800");
    ui->combo_baud->addItem("921600");

    ui->tabWidget->setTabEnabled(1,false);

    connect(shortcut, &QShortcut::activated, this, &MainWindow::toggleWidgets);
    connect(serial, SIGNAL(readyRead()), this, SLOT(ReadyReads()));
    connect(time,SIGNAL(timeout()),this,SLOT(IntervalTimer()));
    connect(udpSocket, &QUdpSocket::readyRead, this, &MainWindow::processPendingDatagrams);
    //    serial->setBaudRate(460800);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    ui->progressBar->setVisible(false);
    ui->btn_listen->setEnabled(false);
    // ui->btn_open->setEnabled(false);
    // ui->btn_program->setEnabled(false);
    // ui->btn_app->setEnabled(false);
    // ui->btn_boot->setEnabled(false);
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


void MainWindow::processPendingDatagrams()
{
    qDebug()<<__LINE__;
    while (udpSocket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(buffer.data(), buffer.size(),&sender , &senderPort);
        qDebug() << "Received from" << sender.toString() << ":" << senderPort;
        for(int i=0;i<buffer.length();i++)
            CheckData(buffer[i]);
    }
}

void MainWindow::SnedData(void)
{
    QByteArray temp;
    quint16 sum=0;

    temp.append(0xc1);
    temp.append(0xb7);
    temp.append('P');
    temp.append(0x15);

    if(index>=binfile.length()/16)
    {
        all2byte.all=index;

        temp.append(2+binfile.length()%16);
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        sum=all2byte.byte[1]+all2byte.byte[0];

        for(int i=0;i<binfile.length()%16;i++)
        {
            sum+=(quint8)binfile.at(index*16+i);
            temp.append(binfile.at(index*16+i));
        }

        all2byte.all=sum;
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        if(ui->rd_serial->isChecked()==true)
        {
            serial->write(temp,temp.length());
        }
        else
        {
            udpSocket->writeDatagram(temp, QHostAddress(ui->lineEdit_4->text()), ui->lineEdit_5->text().toInt(nullptr,10));
        }
        qDebug()<< __LINE__ << temp.toHex(' ');
        temp.clear();
        index++;
        time->stop();
        flag_timeout=false;
        // qDebug()<<__LINE__<<"stop timer";
        ui->progressBar->setValue((100/(float)len)*index);
        // Msg.information(nullptr,"information","send ok",QMessageBox::Ok);
        ui->progressBar->setVisible(false);
        ui->btn_program->setEnabled(true);
        ui->combo_mcu->setEnabled(true);
        ui->btn_app->setEnabled(true);
        ui->btn_boot->setEnabled(true);
        ui->label_2->setText(" ");
        //        sendLength(0x17);
        // qDebug()<<__LINE__<<"--------------------end send";
    }
    else
    {
        all2byte.all=index;

        temp.append(18);
        temp.append(all2byte.byte[0]);
        temp.append(all2byte.byte[1]);
        sum=all2byte.byte[0]+all2byte.byte[1];

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

        if(index>=binfile.length()/16)
        {
            // time->stop();
            flag_timeout=false;
            // qDebug()<<__LINE__<<"stop timer";
            ui->progressBar->setValue((100/(float)len)*index);
            // Msg.information(nullptr,"information","send ok",QMessageBox::Ok);
            ui->progressBar->setVisible(false);
            ui->btn_program->setEnabled(true);
            ui->combo_mcu->setEnabled(true);
            ui->btn_app->setEnabled(true);
            ui->btn_boot->setEnabled(true);
            ui->label_2->setText(" ");
            //            sendLength(0x17);
            // qDebug()<<__LINE__<<"--------------------end send2";
        }


        if(ui->rd_serial->isChecked()==true)
        {
            serial->write(temp,temp.length());
        }
        else
        {
            udpSocket->writeDatagram(temp, QHostAddress(ui->lineEdit_4->text()), ui->lineEdit_5->text().toInt(nullptr,10));
        }

        ui->progressBar->setValue((100/(float)len)*index);
        qDebug()<< __LINE__<< temp.toHex(' ');

    }
}

void MainWindow::AckRecive(QByteArray cmd)
{
    switch (cmd[1])
    {
    case 0x15:{
        qDebug()<< __LINE__<< counter_data << data.toHex(' ');
        quint16 value = 0;
        memcpy(&value, data.data()+3, 2);
        qDebug()<< __LINE__<< ", value:" << (quint16)value << ", index:" << index;
        // flag_timeout=false;
        timeout2=0;
        if( value + 1 == index )
        {
            flag_headers=false;
            flag_send=true;
        }
        break;
    }
    case 0x18:{
        // flag_timeout=false;
        timeout_erase=false;
        timeout2=0;
        //        qDebug()<< __LINE__;
        flag_send=true;
        ui->label_2->setText("Programming...");
        ui->progressBar->setValue(0);
        // ui->btn_open->setEnabled(false);
        ui->btn_program->setEnabled(false);
        ui->combo_mcu->setEnabled(false);
        ui->btn_app->setEnabled(false);
        ui->btn_boot->setEnabled(false);
        flag_timeout=true;
        break;
    }
    case 0x16:{
        timeout2=0;
        index=0;
        //        qDebug()<< __LINE__;
        flag_write=false;
        sendLength(0x18);
        break;
    }
    case 0x19:{
        timeout2=0;
        timeout_erase=true;
        ui->label_2->setText("Erasing...");
        index_erase=cmd[3];
        float a=(100.0/15.0*cmd[3]);
        if(a>=100)a=100;
        //        qDebug()<<__LINE__<<cmd.toHex(' ')<<(float)a;
        ui->progressBar->setValue(a);
        break;
    }
    case 0x20:{
        // flag_timeout=false;
        timeout2=0;
        memcpy(&DeviceID,cmd.data()+3,4);
        qDebug()<<__LINE__<<cmd.toHex(' ');
        qDebug()<<__LINE__<<hex<<DeviceID<<SelectDevice;
        if(SelectDevice==0x450)
        {
            DeviceID&=0xffff;
        }

        if(DeviceID==SelectDevice)
        {
            sendLength(0x16);
            // ui->progressBar->setValue(0);
            // ui->progressBar->setVisible(true);
            // ui->btn_program->setEnabled(false);
            // ui->label_2->setText("Please Reset Micro "+QString::number(timeout,10)+" Second");
            qDebug()<<__LINE__<<"Device ok";
            // index=0;
        }
        else
        {
            flag_write=false;
            flag_ok=false;
            time->stop();
            qDebug()<<__LINE__<<"stop timer";
            // ui->progressBar->setValue(0);
            ui->progressBar->setVisible(false);
            ui->btn_program->setEnabled(true);
            ui->combo_mcu->setEnabled(true);
            ui->label_2->setText("");
            Msg.warning(nullptr,"اخطار","چیپ انتخاب شده اشتباه است",QMessageBox::Ok);
            //            ui->btn_app->setEnabled(false);
            //            ui->btn_boot->setEnabled(false);
            // qDebug()<<__LINE__<<DeviceID;
        }
        break;
    }

    }
}

void MainWindow::CheckData(unsigned char Data)
{
    //    quint8 len_recive=0;
    quint16 sum=0;
    quint16 check_sum=0;
    header2 = header1;
    header1 =(quint8) Data;
    if (header2 == 0xC1 && header1 == 0xB7 && flag_headers==false)
    {
        flag_headers = true;
        counter_data=0;
    }
    else if(flag_headers == true)
    {
        data[counter_data] = Data;

        if(counter_data == 8)
        {

            sum = (quint8)data[5] + (quint8)data[6] + (quint8)data[3] + (quint8)data[4];
            check_sum = 0;
            memcpy(&check_sum,data.data()+7,2);

            qDebug()<<__LINE__<<"Recive"<<data.toHex(' ');
            if( sum == check_sum )
            {
                //timeout2=0;
                timeout=20;
                flag_ok=true;
                //                qDebug()<<__LINE__<<"Recive"<<hex<<data.toHex(' ');
                AckRecive(data);
                data.clear();
            }
            else
            {
                flag_ok=false;
            }
            flag_headers=false;
            counter_data = 0;
            data.clear();
        }
        else
            counter_data++;
    }
}

void MainWindow::IntervalTimer(void)
{
    static int timesec=0,timErase=0;

    val2=val1;
    val1=index;

    // qDebug()<<__LINE__<<val1<<val2<<flag_timeout;
    if((val1==val2 && flag_timeout))
    {
        if(++timeout2>=5)
        {
            flag_timeout=false;
            timeout_erase=false;
            time->stop();
            ui->progressBar->setValue(0);
            ui->progressBar->setVisible(false);
            ui->btn_program->setEnabled(true);
            ui->combo_mcu->setEnabled(true);
            ui->label_2->setText("");
            //            ui->btn_app->setEnabled(false);
            //            ui->btn_boot->setEnabled(false);
            // flag_write=true;
            // time->start(20);
            // flag_ok=false;
            // timeout=20;
            Msg.warning(nullptr,"اخطار","ارتباط قطع شد",QMessageBox::Ok);
        }
    }


    if(timeout_erase)
    {
        if(++timErase>=50)
        {
            timErase=0;
            val4=val3;
            val3=index_erase;
            if(val3==val4)
            {
                timeout_erase=false;
                time->stop();
                ui->progressBar->setValue(0);
                ui->progressBar->setVisible(false);
                ui->btn_program->setEnabled(true);
                ui->combo_mcu->setEnabled(true);
                ui->label_2->setText("");
                //            ui->btn_app->setEnabled(false);
                //            ui->btn_boot->setEnabled(false);
                // flag_write=true;
                // time->start(20);
                // flag_ok=false;
                // timeout=20;
                Msg.warning(nullptr,"اخطار","ارتباط قطع شد",QMessageBox::Ok);
            }
        }
    }

    if(++timesec>=30 && !flag_ok)
    {
        timesec=0;
        timeout--;
        ui->label_2->setText("Please Reset Micro "+QString::number(timeout,10)+" Second");
        if(timeout==0)
        {
            ui->progressBar->setVisible(false);
            ui->btn_program->setEnabled(true);
            ui->combo_mcu->setEnabled(true);
            //            ui->btn_app->setEnabled(true);
            //            ui->btn_boot->setEnabled(true);
            ui->label_2->setText(" ");
            flag_write=false;
            time->stop();
        }
    }

    if(flag_send)
    {
        flag_send=false;
        SnedData();
    }

    if(flag_write)
    {
        sendLength(0x20);
    }
}

void MainWindow::ReadyReads(void)
{
    QByteArray tempdata;
    tempdata=serial->readAll();
    //        qDebug()<<__LINE__<<"Recive serial"<<hex<<tempdata.toHex(' ');
    for(int i=0;i<tempdata.length();i++)
    {
        CheckData(tempdata.at(i));
    }
}

void MainWindow::on_btn_open_clicked()
{
    QByteArray temp;
    quint16 crc2=0;
    quint16 len2;
    QString filePath =/*"C:/Users/RayanRoshd/Desktop/mohsen/b.bin"; */QFileDialog::getOpenFileName(this, "Open Binary File", "", "BIN (*.bin)");
    if (!filePath.isEmpty())
    {
        file.setFileName(filePath);
        file.open(QIODevice::ReadWrite);
        temp=file.readAll();
        file.close();
        len2=temp[0];
        quint16 crc=CRC16_Modbus(temp,len2+1);
        memcpy(&crc2,temp.data()+len2+1,2);

        if(crc==crc2)
        {
            QString str=temp.left(len2+1);
            str=str.remove(0,1);
            binfile=temp.remove(0,len2+3);
            qDebug()<<binfile.size();
            len=binfile.size()/16;
            // qDebug()<<len;

            if((binfile.size()%16)!=0)
                len++;
            ui->label->setText(QString::number(binfile.size()/1024,10)+"KByte"+" Len="+QString::number(len,10)+"\r\n"+str);
            flag_file_valid=true;
        }
        else
        {
            flag_file_valid=false;
            Msg.warning(nullptr,"اخطار","فایل نا معتبر است",QMessageBox::Ok);
        }
    }
    else
    {
        flag_file_valid=false;
        Msg.warning(nullptr,"اخطار","فایلی انتخاب نشده است",QMessageBox::Ok);
    }
}


void MainWindow::on_btn_port_clicked()
{
    serial->setBaudRate(ui->combo_baud->currentText().toInt(nullptr,10));
    qDebug()<<__LINE__<<serial->baudRate();
    serial->setPortName(ui->combo_port->currentText()); //ListStr[0]
    if(serial->isOpen() == false)
    {
        if(serial->open(QIODevice::ReadWrite))
        {
            ui->combo_port->setEnabled (false);
            ui->btn_port->setText("Disconnect");
            ui->combo_baud->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            // ui->btn_open->setEnabled(true);
            // ui->btn_program->setEnabled(true);
            // ui->btn_app->setEnabled(true);
            // ui->btn_boot->setEnabled(true);
        }
        else if(!serial->open(QIODevice::ReadWrite))
        {
            ui->combo_port->setEnabled (true);
            ui->combo_baud->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            // ui->btn_open->setEnabled(false);
            // ui->btn_program->setEnabled(false);
            // ui->btn_app->setEnabled(false);
            // ui->btn_boot->setEnabled(false);
            ui->btn_port->setText("Connect");
            Msg.setText("This serial port is used by another device");
            Msg.exec();
        }
    }
    else if(serial->isOpen() == true)
    {
        serial->close();
        ui->combo_port->setEnabled (true);
        ui->combo_baud->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        // ui->btn_open->setEnabled(false);
        // ui->btn_program->setEnabled(false);
        // ui->btn_app->setEnabled(false);
        // ui->btn_boot->setEnabled(false);
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



void MainWindow::sendLength(quint8 cmd)
{
    QByteArray ba;
    uint16_t sum=0;
    ba.resize(9);
    ba[0] = 0xc1;
    ba[1] = 0xb7;
    ba[2] = 'P';
    ba[3] = cmd;
    ba[4] = 0x2;
    memcpy(ba.data() + 5, &len, 2);
    sum=(quint8)ba[5]+(quint8)ba[6];
    memcpy(ba.data() + 7, &sum, 2);

    qDebug()<<__LINE__<<"Send"<<ba.toHex(' ');
    if(ui->rd_serial->isChecked()==true)
        serial->write(ba, ba.length());
    else
        udpSocket->writeDatagram(ba, QHostAddress(ui->lineEdit_4->text()), ui->lineEdit_5->text().toInt(nullptr,10));
    // qDebug()<<__LINE__<<hex<<cmd;
}

void MainWindow::on_btn_program_clicked()
{
    if(flag_file_valid)
    {
        // flag_file_valid=false;
        if((ui->lineEdit_4->text()=="" || ui->lineEdit_5->text()=="")&& ui->rd_lan->isChecked())
        {
            Msg.warning(nullptr,"اخطار","آی پی یا پورت مقصد مشخص نیست",QMessageBox::Ok);
        }
        else if(serial->isOpen() || udpSocket->isOpen())
        {
            index=0;
            ui->progressBar->setValue(0);
            ui->progressBar->setVisible(true);
            ui->btn_program->setEnabled(false);
            ui->combo_mcu->setEnabled(false);
            //            ui->btn_app->setEnabled(false);
            //            ui->btn_boot->setEnabled(false);
            flag_write=true;
            time->start(20);
            flag_ok=false;
            timeout=20;
            ui->label_2->setText("Please Reset Micro "+QString::number(timeout,10)+" Second");
        }
        else
        {
            Msg.warning(nullptr,"اخطار","پورت سریال باز نیست",QMessageBox::Ok);
        }
    }
    else
        Msg.warning(nullptr,"اخطار","فایلی انتخاب نشده است",QMessageBox::Ok);
}



void MainWindow::on_btn_app_clicked()
{
    sendLength(0x17);
}





void MainWindow::on_btn_boot_clicked()
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xc1;
    ba[1] = 0xb7;
    ba[2] = 'P';
    ba[3] = 0x55;
    ba[4] = 2;
    ba[5] = 0xaa;
    ba[6] = 0x55;
    ba[7] = ba[5]+ba[6];
    ba[8] = 0;
    ba[9] = 0x81;
    if(ui->rd_serial->isChecked()==true)
    {
        serial->write(ba,10);
    }
    else

    {
        udpSocket->writeDatagram(ba, QHostAddress(ui->lineEdit_4->text()), ui->lineEdit_5->text().toInt(nullptr,10));
    }
    qDebug()<<__LINE__<<ba.toHex(' ');
}


void MainWindow::on_btn_convert_clicked()
{

    QString filePath =/*"C:/Users/RayanRoshd/Desktop/mohsen/b.bin";*/ QFileDialog::getOpenFileName(this, "Open Binary File", "", "BIN (*.bin)");
    if (!filePath.isEmpty())
    {
        file.setFileName(filePath);
        file.open(QIODevice::ReadWrite);
        binfile=file.readAll();
        file.close();
        qDebug()<<__LINE__;
        len=binfile.size()/16;
        if((binfile.size()%16)!=0)
            len++;
        ui->label->setText(QString::number(binfile.size(),10)+" Byte"+" Len="+QString::number(len,10));
        if(ui->lineEdit->text()!="" && ui->lineEdit_2->text()!="" && ui->lineEdit_3->text()!="")
        {
            QByteArray temp2;
            quint16 crc=0;
            QString infofile=ui->lineEdit->text()+" "+ui->lineEdit_2->text()+" "+ui->lineEdit_3->text();
            temp2.append(infofile.length());
            temp2.append(infofile.toUtf8());
            crc=CRC16_Modbus(temp2,temp2.length());
            memcpy(temp2.data()+temp2.length(),&crc,2);
            temp2.resize(infofile.length()+3);
            qDebug()<<temp2.toHex(' ');
            binfile.prepend(temp2);
            out.setFileName(QFileDialog::getSaveFileName(this,"Save file","","BIN(*.bin)"));
            out.open(QIODevice::ReadWrite);
            out.write(binfile);
            out.close();
        }
        else
            Msg.warning(nullptr,"اخطار","کادرها نباید خالی باشند",QMessageBox::Ok);
    }
    else
        Msg.warning(nullptr,"اخطار","فایلی انتخاب نشده است",QMessageBox::Ok);
}


uint16_t MainWindow::CRC16_Modbus(QByteArray data, quint16 length)
{
    uint16_t crc = 0xFFFF;
    uint8_t i;
    for (uint16_t j = 0; j < length; j++)
    {
        crc ^= (quint16)data[j];
        for (i = 8; i !=0; i--)
        {
            if ((crc & 0x0001)!=0)
            {
                crc >>= 1;
                crc ^= 0x0815;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}



void MainWindow::on_rd_lan_clicked(bool checked)
{
    if(checked)
    {
        ui->btn_listen->setEnabled(true);
        ui->groupBox_2->setEnabled(false);
        ui->lineEdit_4->setEnabled(true);
        ui->lineEdit_5->setEnabled(true);
        ui->lineEdit_6->setEnabled(true);
        ui->btn_listen->setEnabled(true);
        serial->close();

    }
}


void MainWindow::on_rd_serial_clicked(bool checked)
{
    if(checked)
    {
        ui->groupBox_2->setEnabled(true);
        udpSocket->close();
        ui->lineEdit_4->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
        ui->btn_listen->setEnabled(false);
        ui->btn_listen->setEnabled(false);
    }
}


void MainWindow::on_btn_listen_clicked()
{
    udpSocket->open(QIODevice::ReadOnly);
    if(ui->lineEdit_6->text()!="")
    {
        if (!udpSocket->bind(QHostAddress::AnyIPv4, ui->lineEdit_6->text().toInt(nullptr,10)))
        {
            qDebug()<< __LINE__ << "Failed to bind UDP socket!";
        }
        else
        {
            qDebug()<< __LINE__ << "UDP socket bound on port "<<ui->lineEdit_6->text();
        }
    }
    else
        Msg.warning(nullptr,"اخطار","پورت دریافت مشخص نیست",QMessageBox::Ok);
}


void MainWindow::on_combo_mcu_currentTextChanged(const QString &arg1)
{
    if(arg1=="LPC1768")
        SelectDevice=0x26013F37;
    else if(arg1=="LPC1788")
        SelectDevice=0x281D3F47;
    else if(arg1=="STM32H743")
        SelectDevice=0x450;
    else if(arg1=="TMS320F28377S")
        SelectDevice=0x00ff0400;

    qDebug()<<__LINE__<<hex<<SelectDevice;
}

void MainWindow::toggleWidgets()
{
    bool en=!ui->tabWidget->isTabEnabled(1);
    ui->tabWidget->setTabEnabled(1,en);
}



