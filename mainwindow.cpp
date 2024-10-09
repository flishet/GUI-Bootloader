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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_open_clicked()
{
    QByteArray temp;
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image File", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        image.load(filePath,NULL);
        if (!image.isNull())
        {
            lbl->setPixmap(QPixmap::fromImage(image));
            lbl->adjustSize();
            convert_print(image,&temp);
            qDebug()<<temp.size();
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

    //       imageMono.save("text_image_mono.bmp", "BMP");
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
                qDebug()<<hex<<temp;
                data->append(temp);
                temp=0;
            }
        }
    }
}

