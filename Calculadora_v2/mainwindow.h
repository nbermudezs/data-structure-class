#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <cmath>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QProcess>


using namespace std;
class nivelOperador
{
public:
    int nivel;
    QList<QString> operadores;
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMap<QString,int> cantArgs;
    QMap<char,double> variables;
    QList<nivelOperador> jerarquiaOperadores;
    QList<QString> operadores;
    QList<QString> postfijo;
    QString infijo;
    QSettings settings;
    QProcess plot;
    enum{NOT_USED=INT_MIN,DIGIT,OPERADOR};

    void init();
    double evaluarPostFijo(QList<QString> pos);
    int existeOperador(QString op);
    void meterOperador(QString op);
    void define(QString t);
    void updateVars();
    #define PI 3.14159265

    void metodoChancho();//Este es el que tengo que hacer mas bonito
    QString reemplazarVars(QString p);

private slots:
    void on_btgraph_clicked();
    void on_plotter_clicked();
    void on_CE_clicked();
    void on_tan_2_clicked();
    void on_pIzq_clicked();
    void on_pDer_clicked();
    void on_tan_clicked();
    void on_cos_clicked();
    void on_sen_clicked();
    void on_igual_clicked();
    void on_dot_clicked();
    void on_entre_clicked();
    void on_por_clicked();
    void on_menos_clicked();
    void on_mas_clicked();
    void on_cero_clicked();
    void on_nueve_clicked();
    void on_ocho_clicked();
    void on_siete_clicked();
    void on_seis_clicked();
    void on_cinco_clicked();
    void on_cuatro_clicked();
    void on_tres_clicked();
    void on_dos_clicked();
    void on_uno_clicked();
    //-------------TERMINA METODOS PA ESCRIBIR EN EL LINEDIT
};

#endif // MAINWINDOW_H
