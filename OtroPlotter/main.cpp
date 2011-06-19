#include <QtGui/QApplication>
#include <plotter.h>
#include <QMessageBox>
#include "calculos.h"
#include <iostream>

using namespace std;

void helpMode()
{
    //No se si esto tiene que ser asi
    cout<<"Ha entrado a la ayuda de mi super ploter :)"<<endl
            <<"Desarollado por Nestor A. Bermudez"<<endl
            <<"Ing. Sistemas Computacionales UNITEC"<<endl
            <<"Estructura de Datos. Ing Rogger Vasquez"<<endl<<endl;

    cout<<"Como usar este plotter?"<<endl;
    cout<<"Para poder graficar se debe seguir la siguiente sintaxis:"<<endl
            <<" -p <funcion de x> minX maxX dX"<<endl<<endl;
    cout<<"Por ejemplo: -p 2*X^3-X/2+1 -30 52.4 0.1"<<endl;

    cout<<"Creo que no hay mas ayuda porque es lo unico que hace esta aplicacion :P"<<endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(argc<2)return 1;//argumentos insuficientes
    if(argc==2 && strcmp(argv[1],"-h")==0)
    {
        helpMode();
    }else{
        if(argc==6)
        {
            if(strcmp(argv[1],"-p")!=0)return 2;//switch incorrecto

            Calculos c;
            QString postFijo=c.toPost(QString(argv[2]));
            PlotSettings sets;
            double xi=atof(argv[3]);
            sets.minX=xi;
            double xf=atof(argv[4]);
            sets.maxX=xf;
            double dX=atof(argv[5]);


            c.setPostfijo(postFijo);

            QVector<QPointF> points;
            int max=INT_MIN,min=INT_MAX;
            for(double i=xi;i<xf;i=i+dX)
            {
                int y=c.evaluar(i);
                max=max<y?y:max;
                min=min>y?y:min;
                points.insert(0,QPointF(i,y));
            }
            sets.minY=min-10;
            sets.maxY=max+10;

            plotter *plot=new plotter();
            plot->setPlotSettings(sets);
            plot->setCurveData(0,points);

            plot->show();
        }
    }

    return a.exec();
}

