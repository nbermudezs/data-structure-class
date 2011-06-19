#ifndef MULTIGRAFO_H
#define MULTIGRAFO_H
#include <QMap>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>

class nodo
{
public:
    nodo(){}
    nodo(QString lo){this->lugar=lo;}
    QString lugar;

    //QMap<destino, QMap<dia, QMap<hora, tiempo> > >
    QMap<QString, QMap<QString, QMap<QString, double> > >  info;
};


class Multigrafo
{
public:
    double ** getFloyd(QString dia,QString ho);
    int size(){return this->currentPos;}
    explicit Multigrafo(int tamano);
    double **matrizFloyd;//llenarla con Floyd.
    ~Multigrafo();
    void agregarNodo(QString lo);//solo agrega los nodos, no crea aristas.
    void crearGrafoPara(QString dia,QString ho);//esto solo llenara la de la adyacencias a partir de los nodos.
    void agregarConexion(QString lo,QString ld,QString dia,QString h,double t);
    void borrarConexion(QString lo,QString ld,QString dia,QString h);
    QStringList camino(QString,QString);
    QString getRelaciones(QString dia,QString hora);

private:
    QMap<QString,int> posiciones;//guarda la posicion q tiene en el arreglo el nodo con lugar=QString
    QMap<int,QString> revPos;
    int tamano;
    int currentPos;
    nodo *nodos;
    double **matriz_adyacencias;//para determinada hora y dia,

    double **P;//el P para
    //double INFINITO;
    static const double INFINITO=9999.9;


    void init();
    QStringList lugares;
    QStringList recuperarCamino(int i,int j);
    void mas_corto();

    void Floyd();
    void llenarMatriz(QString dia,QString hora);


};

#endif // MULTIGRAFO_H
