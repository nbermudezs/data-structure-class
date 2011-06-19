#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->escena=new QGraphicsScene(0,0,this->ui->graphicsView->width(),ui->graphicsView->height());
    this->escena->setSceneRect(0,0,this->ui->graphicsView->width(),ui->graphicsView->height());
    this->ui->graphicsView->setScene(escena);
    this->marcado=QColor(Qt::cyan);
    this->limpio=QColor(Qt::white);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCargar_Nodos_triggered()
{

    QString path=QFileDialog::getOpenFileName(this,"Nodos",QDir::currentPath(),"*.txt");
    QString t;
    if(path.isNull())return;
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    if(!f.isOpen())return;
    QString s=f.readLine();s=s.left(s.length()-2);
    int size=s.toInt();
    this->miGrafo=new Multigrafo(size);
    this->escena->clear();int i=1;
    this->ui->listWidget->clear();
    while(!(t=f.readLine()).contains("[ARISTAS]") && f.bytesAvailable())
    {
        t=t.replace(" ","").toUpper();
        double posX=t.left(t.indexOf(",")).toDouble();
        t=t.mid(t.indexOf(",")+1);
        double posY=t.left(t.indexOf(",")).toDouble();
        t=t.mid(t.indexOf(",")+1);
        t=t.left(t.length()-2);
        this->miGrafo->agregarNodo(t);
        this->ui->listWidget->addItem("El "+QString::number(i++)+" corresponde a "+t);
        NodoItem *item=new NodoItem();
        item->setText(t);
        item->setX(posX);item->setY(posY);
        this->escena->addItem(item);

    }

    while(f.bytesAvailable())
    {
        t=f.readLine().toUpper().replace(" ","");
        QString dia=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        QString n1=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        QString n2=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        QString hora=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        t=t.left(t.length()-2);
        this->miGrafo->agregarConexion(n1,n2,dia,hora,t.toDouble());
        this->miGrafo->agregarConexion(n2,n1,dia,hora,t.toDouble());
    }

    //t=f.readAll();//esta y la sig linea son para prueba, luego hay que kitarla permanentemente
    //this->relaciones(t);
    //this->miGrafo->crearGrafoPara("LUNES","0");
}

void MainWindow::relaciones(QString t)
{//Dibujará las aristas entre todos los nodos de el dia y hora pedidos
    algoCambio=true;
    QStringList a=t.toUpper().split("\n",QString::SkipEmptyParts);
    for(int i=0;i<a.count();i++)
    {
        t=a.at(i);
        QString dia=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        QString n1=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        QString n2=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        QString hora=t.left(t.indexOf(","));
        t=t.mid(t.indexOf(",")+1);
        Enlace *link=new Enlace(getNodo(n1),getNodo(n2),t);
        this->escena->addItem(link);
    }
}

NodoItem *MainWindow::getNodo(QString lugar)
{
    QList<QGraphicsItem*> items=this->ui->graphicsView->items();
    foreach(QGraphicsItem *i,items)
    {
        if(dynamic_cast<NodoItem*>(i)->text()==lugar)
            return dynamic_cast<NodoItem*>(i);
    }
    return 0;
}

void MainWindow::on_pushButton_2_clicked()//este es el refresh, se me olvido cambiar el nombre
{
    this->ui->tableWidget->clear();
    while(this->ui->tableWidget->rowCount()>0)this->ui->tableWidget->removeRow(0);
    while(this->ui->tableWidget->columnCount()>0)this->ui->tableWidget->removeColumn(0);
    double **data=this->miGrafo->getFloyd(this->currentDia,this->currentH);
    int t=miGrafo->size();
    for(int i=0;i<t;i++)this->ui->tableWidget->insertRow(i);
    for(int i=0;i<t;i++)this->ui->tableWidget->insertColumn(i);
    QTableWidgetItem *item;
    for(int i=0;i<t;i++)
    {
        for(int j=0;j<t;j++)
        {
            item=new QTableWidgetItem(QString::number(data[i][j]));
            this->ui->tableWidget->horizontalHeader()->resizeSection(j,50);
            this->ui->tableWidget->setItem(i,j,item);
        }
    }
    this->algoCambio=false;
}

void MainWindow::on_dibujar_clicked()
{
    this->limpiarNodos();
    this->ui->ruta->clear();

    //Kita las aristas de grafos previos
    QList<QGraphicsItem *> items=this->escena->items();
    for(int i=0;i<items.count();i++)
        items.at(i)->setSelected(true);
    this->limpiarAristas();

    //Almacena los valores que definieron el grafo
    this->currentDia=this->ui->dia->currentText().toUpper();
    this->currentH=this->ui->hora->currentText().left(this->ui->hora->currentText().indexOf(" "));

    QString relaciones=this->miGrafo->getRelaciones(currentDia,currentH);

    this->relaciones(relaciones);
    items=this->escena->items();

    //Les kita la seleccion a los nodos.
    for(int i=0;i<items.count();i++)
        items.at(i)->setSelected(false);
    this->ui->current->setText("Grafo para el dia "+currentDia+" de las "+currentH+
                               " a las "+QString::number(currentH.toInt()+1)+" horas");
}

void MainWindow::on_actionEliminar_Arista_triggered()
{
    this->algoCambio=true;
    QList<QGraphicsItem *> items = escena->selectedItems();
    QMutableListIterator<QGraphicsItem *> iterador(items);
    while (iterador.hasNext()) {
        Enlace *link = dynamic_cast<Enlace *>(iterador.next());
        if (link)
        {
            this->miGrafo->borrarConexion(link->getNodoOrigen()->text(),link->getNodoDestino()->text(),
                                          this->currentDia,this->currentH);
            delete link;
            iterador.remove();
        }
    }
    int i=0;
    while(i<items.count())
    {
        if(dynamic_cast<NodoItem *>(items.at(i)))
            items.takeAt(i);
        else
            i++;
    }
    qDeleteAll(items);
}

void MainWindow::on_actionAgregar_Arista_triggered()
{
    algoCambio=true;
    QList<QGraphicsItem *> items = escena->selectedItems();
    if(items.count()!=2)
    {
        QMessageBox::information(this,"Error","Debe seleccionar 2 nodos para\nagregar una arista");
        return;
    }
    NodoItem *primero = dynamic_cast<NodoItem *>(items.first());
    NodoItem *segundo = dynamic_cast<NodoItem *>(items.last());
    QString text = QInputDialog::getText(this,
                           "Peso", "Tiempo entre los puntos: ",
                           QLineEdit::Normal);
    if(text.isEmpty() || text.toDouble()==0)return;
    Enlace *link=new Enlace(primero,segundo,text);
    escena->addItem(link);
    this->miGrafo->agregarConexion(primero->text(),segundo->text(),currentDia,currentH,text.toDouble());
    this->miGrafo->agregarConexion(segundo->text(),primero->text(),currentDia,currentH,text.toDouble());
}

void MainWindow::cambioDePeso(QString lo, QString ld,double t)
{
    this->algoCambio=true;
    this->miGrafo->agregarConexion(lo,ld,currentDia,currentH,t);
    this->miGrafo->agregarConexion(ld,lo,currentDia,currentH,t);
}

void MainWindow::on_calcular_clicked()
{

    if(algoCambio)
    {
        QMessageBox::information(this,"Update needed","Refresh Floyd para poder calcular");
        return;
    }
    QString lo=this->ui->lo->text().toUpper();QString ld=this->ui->ld->text().toUpper();
    if(lo.isEmpty() || ld.isEmpty())return;
    this->limpiarNodos();
    QStringList t=this->miGrafo->camino(lo,ld);
    this->ui->ruta->clear();
    this->ui->ruta->appendPlainText("----------Ruta mas corta---------\n\n");
    for(int i=0;i<t.count();i++)
    {
        this->ui->ruta->appendPlainText("\t"+t.at(i));
    }

    for(int i=0;i<t.count()-1;i++)this->pintarLugar(t.at(i),marcado);
}

void MainWindow::pintarLugar(QString t, QColor c)
{
    QList<QGraphicsItem*> items=this->escena->items();
    foreach(QGraphicsItem * it,items)
    {
        if(dynamic_cast<NodoItem *>(it)!=0)
            if(dynamic_cast<NodoItem *>(it)->text()==t){
                dynamic_cast<NodoItem *>(it)->setBackgroundColor(c);
                break;
            }
    }
}

void MainWindow::limpiarNodos()
{
    QList<QGraphicsItem*> items=this->escena->items();
    foreach(QGraphicsItem * it,items)
    {
        if(dynamic_cast<NodoItem *>(it)!=0)
            dynamic_cast<NodoItem *>(it)->setBackgroundColor(limpio);
    }
}

void MainWindow::on_actionModificar_Arista_triggered()
{
    QList<QGraphicsItem *> items=this->escena->selectedItems();
    if(items.count()>1 || items.count()==0)
    {
        QMessageBox::information(this,"Error","Debe seleccionar exactamente una arista");
        return;
    }

    if(!dynamic_cast<Enlace *>(items.at(0)))
    {
        QMessageBox::information(this,"Error","No es una arista");
        return;
    }
    QString text = QInputDialog::getText(this,
                           "Nuevo Peso", "Tiempo entre los puntos: ",
                           QLineEdit::Normal);
    if(text.isEmpty() || text.toDouble()==0)return;
    dynamic_cast<Enlace *>(items.at(0))->setPeso(text);
    QString lo=dynamic_cast<Enlace *>(items.at(0))->getNodoDestino()->text();
    QString ld=dynamic_cast<Enlace *>(items.at(0))->getNodoOrigen()->text();
    this->cambioDePeso(lo,ld,text.toDouble());

}

void MainWindow::limpiarAristas()
{
    this->algoCambio=true;
    QList<QGraphicsItem *> items = escena->selectedItems();
    QMutableListIterator<QGraphicsItem *> iterador(items);
    while (iterador.hasNext()) {
        Enlace *link = dynamic_cast<Enlace *>(iterador.next());
        if (link)
        {
            delete link;
            iterador.remove();
        }
    }
    int i=0;
    while(i<items.count())
    {
        if(dynamic_cast<NodoItem *>(items.at(i)))
            items.takeAt(i);
        else
            i++;
    }
    qDeleteAll(items);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
        switch(QMessageBox::warning( ui->centralWidget,
                                     "Saliendo",
                                     "¿Desea guardar el grafo antes de salir?",
                                     QMessageBox::No | QMessageBox::Yes | QMessageBox::Cancel,
                                     QMessageBox::Yes ))
        {
        case QMessageBox::Yes:
            this->salvar();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        default:
            e->ignore();
        }

}

void MainWindow::salvar()
{
    QString f=QFileDialog::getSaveFileName(this,"Guardar Grafo",QDir::currentPath(),"*.txt");
    if(f.isNull())return;
    QFile file(f);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())return;
    QTextStream handler(&file);

    QString text;
    QStringList t;t<<"LUNES"<<"MARTES"<<"MIERCOLES"<<"JUEVES"<<"VIERNES"<<"SABADO"<<"DOMINGO";
    QList<QGraphicsItem*> items=this->escena->items();
    text.append(QString::number(miGrafo->size())+"\n");
    for(int i=0;i<items.count();i++)
    {
        if(dynamic_cast<NodoItem *>(items.at(i)))
        {
            text.append(QString::number(items.at(i)->x())+","+QString::number(items.at(i)->y())+","+
                        dynamic_cast<NodoItem *>(items.at(i))->text()+"\n");
        }
    }
    text.append("[ARISTAS]\n");
    for(int i=0;i<t.count();i++)
    {
        for(int j=0;j<24;j++)
        {
            text.append(this->miGrafo->getRelaciones(t.at(i),QString::number(j)));
        }
    }
    handler<<text;
    file.close();

}

void MainWindow::on_actionAbout_Developer_triggered()
{

    QMessageBox::about(this,"About",
                       "Aplicacion creada por Néstor A. Bermúdez.\n\nProyecto de Estructura de Datos\n"
                       "Catedratico: Ing. Rogger Vasquez\n"
                       "Ingeniería en Sistemas Computacionales\n"
                       "Aplicacion del algoritmo de Floyd, Recuperacion de caminos\n"
                       "Conceptos graficos en QT y de multigrafos\n\n"
                       "Sugerencias o comentarios a: nestor.bermudez@unitec.edu");
}

void MainWindow::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this,"About QT");
}

void MainWindow::on_actionSalvar_triggered()
{
    this->salvar();
}
