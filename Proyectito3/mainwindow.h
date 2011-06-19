#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsEllipseItem>
#include "multigrafo.h"
#include "nodoitem.h"
#include "enlace.h"

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
    Multigrafo *miGrafo;
    QGraphicsScene *escena;
    NodoItem * getNodo(QString lugar);
    QString currentDia,currentH;
    void pintarLugar(QString,QColor);
    QColor marcado,limpio;
    void limpiarNodos();
    bool algoCambio;
    void relaciones(QString t);
    void limpiarAristas();
    void salvar();
    void closeEvent(QCloseEvent *);

public slots:
    void cambioDePeso(QString lo,QString ld,double t);

private slots:
    void on_actionSalvar_triggered();
    void on_actionAbout_QT_triggered();
    void on_actionAbout_Developer_triggered();
    void on_actionModificar_Arista_triggered();
    void on_calcular_clicked();
    void on_actionAgregar_Arista_triggered();
    void on_actionEliminar_Arista_triggered();
    void on_dibujar_clicked();
    void on_pushButton_2_clicked();
    void on_actionCargar_Nodos_triggered();
};

#endif // MAINWINDOW_H
