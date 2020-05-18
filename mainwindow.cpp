#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Menú");
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(showTime()));
    timer->start();

    QDateTime dateTime=QDateTime::currentDateTime();
    QString datetimetex=dateTime.toString("dd/MM/yyyy");
    ui->fecha->setText(datetimetex);

}


void MainWindow::showTime()
{
    QTime time=QTime::currentTime();
    QString time_text=time.toString("hh : mm : ss");
    ui->hora->setText(time_text);
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_datos_clicked()
{
    hide();
    carnet = new Carnet(this);
    carnet->show();

}



void MainWindow::on_salir_clicked()
{
    close();
}
