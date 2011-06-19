#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->maxLen=0;
    this->on_actionAbout_triggered();
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
    //this->ui->tableWidget->resize(15*size,15*size);

    QTableWidgetItem *item;
    this->ui->tableWidget->clear();
    for(int a=0;a<size;a++)this->ui->tableWidget->insertColumn(this->ui->tableWidget->columnCount());
    for(int row=0;row<size;row++)
    {this->ui->tableWidget->insertRow(this->ui->tableWidget->rowCount());

        puzzle[row]=(char *) malloc(size * sizeof(char *));
        for(int i=0;i<size;i++){
            this->puzzle[row][i]=firstLine.at(i).toAscii();
            item=new QTableWidgetItem(firstLine.at(i));
            this->ui->tableWidget->horizontalHeader()->resizeSection(i,15);

            this->ui->tableWidget->setItem(row,i,item);
        }firstLine=file.readLine().replace(" ","");
        this->ui->tableWidget->verticalHeader()->resizeSection(row,15);
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
        //kilometricString.append(" "+n);
        if(maxLen<n.count())maxLen=n.count();
        kilometricString.append(" "+n+" "+rev);
        this->ui->listWidget->addItem(n);
    }
    kilometricString.append(" ");
    this->minLen=3;
}

QString MainWindow::reverse(QString pal)
{
    QString n;
    for(int i=0;i<pal.count();i++)n.prepend(pal.at(i));
    return n;
}

void MainWindow::solucionar()
{
    QString hori;
    this->palabras.clear();
    QTime ti;
    ti.start();
    int flag=0;int pos;
    for(int xInicio=0;xInicio<size;xInicio++){
        flag=0;
        for(int yInicio=0;yInicio<size;yInicio++){
            int hoL=minLen;
            while(true)
            {
                if(hoL>maxLen)break;
                hori=sigPalabra(xInicio,yInicio,hoL,HORIZONTAL);
                if((pos=kilometricString.indexOf(hori))!=-1){
                    if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                        this->palabras.push_back(hori);
                    }
                    hoL++;
                    continue;
                }
                else{
                    hori=reverse(hori);
                    if((pos=kilometricString.indexOf(hori))!=-1){
                        if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                            this->palabras.push_back(hori);
                        }
                        hoL++;
                        continue;
                    }else
                        break;
                }
            }
            hoL=minLen;
            while(true)
            {
                if(hoL>maxLen)break;
                hori=sigPalabra(xInicio,yInicio,hoL,VERTICAL);
                if((pos=kilometricString.indexOf(hori))!=-1){
                    if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                        this->palabras.push_back(hori);
                    }
                    hoL++;
                    continue;
                }
                else{
                    hori=reverse(hori);
                    if((pos=kilometricString.indexOf(hori))!=-1){
                        if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                            this->palabras.push_back(hori);
                        }
                        hoL++;
                        continue;
                    }else
                        break;
                }
            }
            hoL=minLen;
            while(true)
            {
                if(hoL>maxLen)break;
                hori=sigPalabra(xInicio,yInicio,hoL,OBLICUA1);
                if((pos=kilometricString.indexOf(hori))!=-1){
                    if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                        this->palabras.push_back(hori);
                    }
                    hoL++;
                    continue;
                }
                else{
                    hori=reverse(hori);
                    if((pos=kilometricString.indexOf(hori))!=-1){
                        if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                            this->palabras.push_back(hori);
                        }
                        hoL++;
                        continue;
                    }else
                        break;
                }
            }
            hoL=minLen;
            while(true)
            {
                if(hoL>maxLen)break;
                hori=sigPalabra(xInicio,yInicio,hoL,OBLICUA2);
                if((pos=kilometricString.indexOf(hori))!=-1){
                    if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                        this->palabras.push_back(hori);
                    }
                    hoL++;
                    continue;
                }
                else{
                    hori=reverse(hori);
                    if((pos=kilometricString.indexOf(hori))!=-1){
                        if(kilometricString.at(pos+1)==' ' && kilometricString.at(pos-hori.count())==' '){
                            this->palabras.push_back(hori);
                        }
                        hoL++;
                        continue;
                    }else
                        break;
                }
            }
        }
    }
    qDebug()<<"Elapse"<<ti.elapsed();
    this->ui->listWidget_2->addItems(this->palabras);
    //qDebug()<<this->ui->listWidget_2->count();

}

void MainWindow::solucionar2()
{
    QTime t;t.start();
    /*for(int j=0;j<size;j++)
    {
        int y=j;
        for(int x=0;x<size;x++)
        {
            int len=this->minLen;
            if(y+len<=size)
            {
                QString pal;pal.append(puzzle[x][y]).append(puzzle[x][y+1]).append(puzzle[x][y+2]);
                y=y+len;
                do
                {
                    if(kilometricString.contains(pal))
                    {
                        if(kilometricString.contains(" "+pal+" "))
                            palabras.push_back(pal);
                        pal.append(puzzle[x][y]);
                        continue;

                    }else
                        break;
                }while(true);
            }

        }
    }*/
    qDebug()<<kilometricString;
    qDebug()<<t.elapsed();
    this->ui->listWidget_2->addItems(palabras);
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
    }
    return pal;
}

void MainWindow::on_pushButton_3_clicked()
{
    //this->solucionar();
    this->solucionar2();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About",
                       "Aplicacion creada por Néstor A. Bermúdez.\n\nProyecto de Estructura de Datos\n"
                       "Catedratico: Ing. Rogger Vasquez\n"
                       "Ingeniería en Sistemas Computacionales\n"
                       "Puzzle Solver\n"
                       "Encontrar todas las palabras en una sopa de letras dada\n\n"
                       "Sugerencias o comentarios a: nestor.bermudez@unitec.edu");
}
