#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->gridLayout->setSpacing(1);
    this->maxLen=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString nombre=QFileDialog::getOpenFileName(this,"File",QDir::currentPath(),"*.txt");

    fillGrid(nombre);
}

void MainWindow::fillGrid(QString nombre)
{
    QFile file(nombre);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen())
        return;
    QString firstLine=file.readLine().replace(" ","");
    this->size=firstLine.count()-2;
    puzzle=(char **) malloc(size * sizeof(char *));

    for(int row=0;row<size;row++)
    {
        puzzle[row]=(char *) malloc(size * sizeof(char *));
        for(int i=0;i<size;i++){
            this->puzzle[row][i]=firstLine.at(i).toAscii();
            QLabel *nueva=new QLabel(firstLine.at(i));
            nueva->setMinimumSize(10,10);
            this->ui->gridLayout->addWidget(nueva,row,i);
        }firstLine=file.readLine().replace(" ","");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString nombre=QFileDialog::getOpenFileName(this,"Dictionary",QDir::currentPath(),"*.txt");
    QFile file(nombre);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen())
        return;
    this->ui->listWidget->clear();
    QString n,rev;
    while(file.bytesAvailable()!=0){
        n=file.readLine().toUpper();
        diccionario.insert(n=n.left(n.length()-2),n);
        diccionario.insert(rev=reverse(n),n);
        if(QString(n).length()>maxLen)maxLen=QString(n).length();
        if(QString(n).length()<minLen)minLen=QString(n).length();
        this->ui->listWidget->addItem(n);
    }
    this->minLen=4;
}

QString MainWindow::reverse(QString pal)
{
    QString n;
    for(int i=0;i<pal.count();i++)n.prepend(pal.at(i));
    return n;
}

void MainWindow::solucionar()
{
    QString hori,verti,obli,obli2;
    this->palabras.clear();
    QTime ti;
    ti.start();
    for(int xInicio;xInicio<size;xInicio++){
        for(int yInicio=0;yInicio<size;yInicio++){
            for(int len=this->minLen;len<=maxLen;len++){
                hori=sigPalabra(xInicio,yInicio,len,HORIZONTAL);
                verti=sigPalabra(xInicio,yInicio,len,VERTICAL);
                obli=sigPalabra(xInicio,yInicio,len,OBLICUA1);
                obli2=sigPalabra(xInicio,yInicio,len,OBLICUA2);
                if(this->diccionario.contains(hori))this->palabras.push_back(diccionario[hori]);
                if(this->diccionario.contains(verti))this->palabras.push_back(diccionario[verti]);
                if(this->diccionario.contains(obli))this->palabras.push_back(diccionario[obli]);
                if(this->diccionario.contains(obli2))this->palabras.push_back(diccionario[obli2]);
            }
        }
    }qDebug()<<"Elapse"<<ti.elapsed();
    this->ui->listWidget_2->addItems(this->palabras);
    qDebug()<<this->ui->listWidget_2->count();

}

QString MainWindow::sigPalabra(int xInicio, int yInicio, int len, int DIR)
{
    QString pal;
    switch(DIR)
    {
    case HORIZONTAL:
        if(len+xInicio>this->size)return " ";
        for(int i=0;i<len;i++)
            pal=pal.append(puzzle[xInicio+i][yInicio]);
        break;
    case VERTICAL:
        if(len+yInicio>this->size)return " ";
        for(int i=0;i<len;i++)
            pal=pal.append(puzzle[xInicio][yInicio+i]);
        break;
    case OBLICUA1:
        if(len+yInicio>this->size || len+xInicio>this->size)return " ";
        for(int i=0;i<len;i++)
            pal=pal.append(puzzle[xInicio+i][yInicio+i]);
        break;
    case OBLICUA2:
        if(len+yInicio>this->size || xInicio-len<0)return " ";
        for(int i=0;i<len;i++)
            pal=pal.append(puzzle[xInicio-i][yInicio+i]);
        break;
    }return pal;
}

void MainWindow::on_pushButton_3_clicked()
{
    this->solucionar();
}
