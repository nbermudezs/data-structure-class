#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <QHash>
#include <QTime>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <setjmp.h>

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
    void fillGrid(QString);
    QString kilometricString;
    char **puzzle;
    int size;
    QHash<QString,QString> diccionario;
    int minLen,maxLen;
    QString reverse(QString);
    enum {VERTICAL,HORIZONTAL,OBLICUA1,OBLICUA2};
    void solucionar();
    void solucionar2();
    QString sigPalabra(int xInicio,int yInicio,int len,int DIR);
    QStringList palabras;

private slots:
    void on_actionAbout_triggered();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
