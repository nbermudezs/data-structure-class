#include "multigrafo.h"

Multigrafo::Multigrafo(int tamano)
{
    this->tamano=tamano;
    this->init();
    this->currentPos=0;
}

Multigrafo::~Multigrafo()
{
    delete []this->matrizFloyd;
    delete []this->nodos;
    delete []this->P;
    delete []this->matriz_adyacencias;
}

void Multigrafo::init()
{
    this->nodos=new nodo[tamano];
    this->matrizFloyd=new double*[tamano];
    this->matriz_adyacencias=new double*[tamano];
    this->P=new double*[tamano];
    for(int i=0;i<tamano;i++){
        matrizFloyd[i]=new double[tamano];
        matriz_adyacencias[i]=new double[tamano];
        P[i]=new double[tamano];
    }
    for(int i=0;i<tamano;i++)
        for(int j=0;j<tamano;j++)
            matriz_adyacencias[i][j]=this->INFINITO;
    for(int i=0;i<tamano;i++)matriz_adyacencias[i][i]=0;

}


QStringList Multigrafo::recuperarCamino(int i, int j)
{
    int k=P[i][j];
    QStringList n;
    if(k==0) return n;
    n.append(recuperarCamino(i,k));
    n.append(revPos[k]);
    n.append(recuperarCamino(k,j));
    return n;

}

void Multigrafo::mas_corto()
{
    //Pre requisito: matriz_adyacencias debe estar llena
    for(int i=0;i<currentPos;i++)
    {
        for(int j=0;j<currentPos;j++)
        {
            matrizFloyd[i][j]=matriz_adyacencias[i][j];
            P[i][j]=0;
        }
    }

    for(int k=0;k<currentPos;k++)
        for(int i=0;i<currentPos;i++)
            for(int j=0;j<currentPos;j++)
                if(matrizFloyd[i][k]+matrizFloyd[k][j]<matrizFloyd[i][j]){
                    matrizFloyd[i][j]=matrizFloyd[i][k]+matrizFloyd[k][j];
                    P[i][j]=k;
                }
}

void Multigrafo::agregarNodo(QString lo)
{
    if(currentPos>=tamano)return;
    this->nodos[currentPos].lugar=lo;
    this->posiciones[lo]=currentPos;
    this->revPos[currentPos]=lo;
    currentPos++;
}

void Multigrafo::Floyd()
{
    for(int i=0;i<currentPos;i++)
        for(int j=0;j<currentPos;j++)
            matrizFloyd[i][j]=matriz_adyacencias[i][j];

    for(int k=0;k<currentPos;k++)
        for(int i=0;i<currentPos;i++)
            for(int j=0;j<currentPos;j++)
                if(matrizFloyd[i][k]+matrizFloyd[k][j]<matrizFloyd[i][j])
                    matrizFloyd[i][j]=matrizFloyd[i][k]+matrizFloyd[k][j];
}

void Multigrafo::crearGrafoPara(QString dia, QString ho)
{
    for(int i=0;i<currentPos;i++)
    {
        QList<QString> destinos=nodos[i].info.keys();
        for(int j=0;j<destinos.count();j++)
        {
            this->matriz_adyacencias[i][posiciones[destinos.at(j)]]=
                    (this->nodos[i]).info[destinos.at(j)][dia].value(ho,double(INFINITO));
            //encontre esto del value(key,defaultValue) por lo del problema que el map mete
            //algo a fuerzas (0 por lo general) si no tiene algo que se accede
        }
    }
    for(int i=0;i<currentPos;i++)
        matriz_adyacencias[i][i]=0;
}

void Multigrafo::agregarConexion(QString lo, QString ld, QString dia, QString h, double t)
{
    for(int i=0;i<currentPos;i++)
    {
        if(nodos[i].lugar==lo)
        {
            nodos[i].info[ld][dia][h]=t;
        }
    }
}

double ** Multigrafo::getFloyd(QString dia,QString ho)
{
    this->crearGrafoPara(dia,ho);
    this->Floyd();
    return this->matrizFloyd;
}

void Multigrafo::borrarConexion(QString lo, QString ld, QString dia, QString h)
{
    nodos[posiciones[lo]].info[ld][dia][h]=this->INFINITO;
    nodos[posiciones[ld]].info[lo][dia][h]=this->INFINITO;
}

QStringList Multigrafo::camino(QString lo,QString ld)
{
    this->mas_corto();
    QStringList t=recuperarCamino(posiciones[lo],posiciones[ld]);
    if(t.count()==0 && matriz_adyacencias[posiciones[lo]][posiciones[ld]]==INFINITO)return QStringList()<<"No hay forma de llegar";
    else
    {
        t.prepend(lo);
        t.append(ld);
        t.append("\nTiempo total del recorrido: "+QString::number(matrizFloyd[posiciones[lo]][posiciones[ld]]));
        return t;
    }
}

QString Multigrafo::getRelaciones(QString dia, QString hora)
{
    QString final;
    for(int i=0;i<currentPos;i++)
    {
        QString lo=revPos[i];
        for(int j=0;j<i;j++)
        {            
            QString ld=revPos[j];
            double val=nodos[i].info[ld][dia].value(hora,double(INFINITO));
            if(val==INFINITO){
                continue;
            }
            else
            {
                final.append(dia+","+lo+","+ld+","+hora+","+QString::number(val)+"\n");
            }
        }
    }
    return final.toAscii();
}
