#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QList>

namespace Ui {
    class MainWindow;
}

class nivelOperador
{
public:
    int nivel;
    QList<QString> operadores;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QMap<QString,int> cantArgs;
    Ui::MainWindow *ui;
    QString infijo;
    QList<QString> operadores;
    QList<QString> postfijo;
    QMap<char,double> variables;
    enum {DIGIT,OPERADOR,VAR};
    QList<nivelOperador> jerarquiaOperadores;


    //Mayor valor de nivel representa menor jerarquia
    int existeOperador(QString op);
    void meterOperador(QString op);
    double evaluarPostfijo(QList<QString> post);

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
