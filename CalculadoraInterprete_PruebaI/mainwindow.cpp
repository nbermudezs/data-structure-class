#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nivelOperador nivel;
    nivel.nivel=0;
    nivel.operadores.push_back("(");
    nivel.operadores.push_back(")");
    qDebug()<<nivel.operadores.count();
    this->jerarquiaOperadores.push_back(nivel);
    nivel.operadores.clear();
    nivel.nivel=1;
    nivel.operadores.push_back("*");
    nivel.operadores.push_back("/");
    this->jerarquiaOperadores.push_back(nivel);;
    nivel.nivel=2;
    nivel.operadores.clear();
    nivel.operadores.push_back("+");
    nivel.operadores.push_back("-");
    this->jerarquiaOperadores.push_back(nivel);
    //Podria usar algo asi pa saber si es unario o binario pero creo k no vale la pena //
    //cantArgs.insert("*",2);
    //cantArgs.insert("/",2);
    //cantArgs.insert("+",2);
    //cantArgs.insert("-",2);
    //cantArgs.insert("sen",1);//Solo pa acordarme para k es
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->operadores.clear();
    this->postfijo.clear();
    if(this->ui->lineEdit->text().trimmed()=="")return;
    else
        this->infijo=this->ui->lineEdit->text().replace(" ","");
    this->postfijo.clear();
    this->ui->lineEdit->clear();
    if(infijo.count("(")!=infijo.count(")"))
    {
        this->ui->lineEdit_2->setText("Missing Parentesis");
        return;
    }
    infijo="("+infijo+")";
    QString currentToken;
    int flag;
    while(infijo.length()!=0)
    {
        currentToken=infijo.at(0);

        qDebug()<<"Current Token: "<<currentToken;
        infijo.remove(0,1);
        if(currentToken==")")
        {
            meterOperador(")");
            continue;
        }
        if(currentToken.at(0).isDigit() || currentToken.at(0)=='/.')
            flag=DIGIT;
        //if(variables.contains(currentToken.at(0)))//Esta validacion no funcionara
          //  flag=VAR;
        else
            flag=OPERADOR;//Luego se validara si es valido o no, aki supongamos k si

        for(;;)//Algo k aprendi :) esto es un equivalente a while(true)
        {
            if(infijo.length()==0)break;
            if(flag==DIGIT && infijo.at(0).isDigit() || infijo.at(0)=='/.'){//Tokeniza numeros
                currentToken.append(infijo.at(0));
                infijo.remove(0,1);
            }else if(flag==VAR)break;//xk solo soportaran 1 letra (el primer at(0))
            else if(flag==OPERADOR && !infijo.at(0).isDigit())
            {
                if(infijo.at(0)!='('){
                    currentToken.append(infijo.at(0));
                    infijo.remove(0,1);
                }else
                {
                    break;
                }
            }else
                break;

        }
        if(flag==DIGIT){
            this->postfijo.push_front(currentToken);
        }
        if(flag==OPERADOR && existeOperador(currentToken)!=-1)
        {

            meterOperador(currentToken);
        }
        if(flag==OPERADOR && existeOperador(currentToken)==-1)
        {
            this->ui->lineEdit_2->setText("Sintax Error!!");
            return;
        }

    }

    this->ui->lineEdit_2->clear();
    for(int k=this->postfijo.count()-1;k>=0;k--)
        this->ui->lineEdit_2->setText(this->ui->lineEdit_2->text().append(postfijo.at(k)+" "));

    QString res=QString::number(this->evaluarPostfijo(postfijo));

    if(res=="inf")
        this->ui->lineEdit_3->setText("Division por cero");
    else
        this->ui->lineEdit_3->setText(res);


}


int MainWindow::existeOperador(QString op)
{
    for(int i=0;i<this->jerarquiaOperadores.count();i++)
    {
        for(int j=0;j<this->jerarquiaOperadores.at(i).operadores.count();j++)
            if(jerarquiaOperadores.at(i).operadores.at(j)==op)
                return jerarquiaOperadores.at(i).nivel;
    }
    return -1;
}

void MainWindow::meterOperador(QString op)
{
    if(operadores.count()==0)
    {
        operadores.push_back(op);
        return;
    }
    if(op==")")
    {
        while(operadores.at(0)!="(")
        {
            postfijo.push_front(operadores.takeAt(0));
        }
        operadores.removeAt(0);
        return;
    }
    if(existeOperador(op)>=existeOperador(operadores.at(0)) && operadores.at(0)!="(")
    {
        postfijo.push_front(operadores.takeAt(0));
        meterOperador(op);

    }else
        operadores.push_front(op);

}

double MainWindow::evaluarPostfijo(QList<QString> post)
{
    int i=post.count()-1;
    while(post.count()!=1)
    {
        QString op=post.at(i);
        if(existeOperador(op)!=-1)
        {
            if(op=="+")
            {
                post[i]=QString::number(post.at(i+2).toDouble()+post.at(i+1).toDouble());
                post.removeAt(i+1);
                post.removeAt(i+1);
            }
            if(op=="-")
            {
                post[i]=QString::number(post.at(i+2).toDouble()-post.at(i+1).toDouble());
                post.removeAt(i+1);
                post.removeAt(i+1);
            }
            if(op=="*")
            {
                post[i]=QString::number(post.at(i+2).toDouble()*post.at(i+1).toDouble());
                post.removeAt(i+1);
                post.removeAt(i+1);
            }
            if(op=="/")
            {
                post[i]=QString::number(post.at(i+2).toDouble()/post.at(i+1).toDouble());
                post.removeAt(i+1);
                post.removeAt(i+1);
            }

        }i--;
    }
    return post.at(0).toDouble();
}











