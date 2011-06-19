#ifndef CALCULOS_H
#define CALCULOS_H

#include <QString>
#include <QStringList>
#include <cmath>
#include <QDebug>
#include <QMessageBox>

class nivelOperador
{
public:
    int nivel;
    QList<QString> operadores;
};

class Calculos
{
public:
    Calculos();
    void setPostfijo(QString p){this->postFijo=p;}
    QString getPost(){return this->postFijo;}
    double evaluar(double val);
    QString toPost(QString p);
    QList<QString> operadores;
    QList<QString> postfijo;
    enum{NOT_USED=INT_MIN,DIGIT,OPERADOR};
    QString infijo;
    void meterOperador(QString op);

private:
    QString postFijo;
    double evaluarPostFijo(QStringList p);
    int existeOperador(QString op);
    QList<nivelOperador> jerarquiaOperadores;

    static const double PI=3.14159263;
};

#endif // CALCULOS_H
