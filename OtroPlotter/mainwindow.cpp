#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    escena=new QGraphicsScene(this);
    escena->setSceneRect(-300,-300,600,600);
    this->ui->graphicsView->setScene(escena);

}

MainWindow::~MainWindow()
{
    delete ui;
}

