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

    void on_btn_convert_clicked();

    void convert_print(QImage image,QByteArray *data);

private:
    Ui::MainWindow *ui;
    QImage image;


    QByteArray img;
    QLabel *lbl;


};
#endif // MAINWINDOW_H
