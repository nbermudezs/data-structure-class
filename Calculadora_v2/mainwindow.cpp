#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->init();
    this->on_actionAbout_triggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//SOLO PARA ESCRIBIR
void MainWindow::on_uno_clicked()
{
    this->ui->expression->insert("1");
}


void MainWindow::on_dos_clicked()
{
    this->ui->expression->insert("2");
}

void MainWindow::on_tres_clicked()
{
    this->ui->expression->insert("3");
}

void MainWindow::on_cuatro_clicked()
{
    this->ui->expression->insert("4");
}

void MainWindow::on_cinco_clicked()
{
    this->ui->expression->insert("5");
}

void MainWindow::on_seis_clicked()
{
    this->ui->expression->insert("6");
}

void MainWindow::on_siete_clicked()
{
    this->ui->expression->insert("7");
}

void MainWindow::on_ocho_clicked()
{
    this->ui->expression->insert("8");
}

void MainWindow::on_nueve_clicked()
{
    this->ui->expression->insert("9");
}

void MainWindow::on_cero_clicked()
{
    this->ui->expression->insert("0");
}

void MainWindow::on_mas_clicked()
{
    this->ui->expression->insert("+");
}

void MainWindow::on_menos_clicked()
{
    this->ui->expression->insert("-");
}

void MainWindow::on_por_clicked()
{
    this->ui->expression->insert("*");
}

void MainWindow::on_entre_clicked()
{
    this->ui->expression->insert("/");
}

void MainWindow::on_dot_clicked()
{
    this->ui->expression->insert(".");
}

void MainWindow::on_igual_clicked()
{
    QString text=this->ui->expression->text();
    if(text.startsWith("define "))
        this->define(text);
    else
        this->metodoChancho();
}

void MainWindow::on_sen_clicked()
{
    this->ui->expression->insert("sen(");
}

void MainWindow::on_cos_clicked()
{
    this->ui->expression->insert("cos(");
}

void MainWindow::on_tan_clicked()
{
    this->ui->expression->insert("^");
}

void MainWindow::on_pDer_clicked()
{
    this->ui->expression->insert("(");
}

void MainWindow::on_pIzq_clicked()
{
    this->ui->expression->insert(")");
}

void MainWindow::on_tan_2_clicked()
{
    this->ui->expression->insert("define ");this->ui->expression->setFocus();
}

void MainWindow::on_CE_clicked()
{
    this->ui->expression->clear();
}

void MainWindow::on_plotter_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"Plotter location...",QDir::currentPath(),"*.exe");
    if(!path.isEmpty())
    {
        this->settings.setValue(QString("Plotter"),path);
    }
}

void MainWindow::on_btgraph_clicked()
{
    QString fx=this->ui->fx->text().toUpper();
    fx=reemplazarVars(fx);
    this->plot.start(this->settings.value("Plotter").value<QString>()+QString(" -p "+fx+"  "+
                                                                              this->ui->min->text()+" "+
                                                                              this->ui->max->text()+" "+
                                                                              this->ui->dx->text()));
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void MainWindow::init()
{
    for(int _='A';_<'Z';_++)
        this->variables.insert(_,NOT_USED);
    this->variables.remove('X');

    nivelOperador nivel;
    nivel.nivel=0;
    nivel.operadores.push_back("(");
    nivel.operadores.push_back(")");
    this->jerarquiaOperadores.push_back(nivel);

    nivel.operadores.clear();
    nivel.nivel++;
    nivel.operadores.push_back("sen");
    nivel.operadores.push_back("cos");
    nivel.operadores.push_back("tan");
    nivel.operadores.push_back("^");
    this->jerarquiaOperadores.push_back(nivel);

    nivel.operadores.clear();
    nivel.nivel++;
    nivel.operadores.push_back("*");
    nivel.operadores.push_back("/");
    this->jerarquiaOperadores.push_back(nivel);

    nivel.nivel++;
    nivel.operadores.clear();
    nivel.operadores.push_back("+");
    nivel.operadores.push_back("-");
    this->jerarquiaOperadores.push_back(nivel);

}

double MainWindow::evaluarPostFijo(QList<QString> post)
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
            if(op=="sen")
            {
                post[i]=QString::number(sin(post.at(i+1).toDouble()/180*PI));
                post.removeAt(i+1);
            }
            if(op=="cos")
            {
                post[i]=QString::number(cos(post.at(i+1).toDouble()/180*PI));
                post.removeAt(i+1);
            }
            if(op=="tan")
            {

                post[i]=QString::number(tan(post.at(i+1).toDouble()/180*PI));
                post.removeAt(i+1);
            }
            if(op=="^")
            {
                post[i]=QString::number(pow(post.at(i+2).toDouble(),post.at(i+1).toDouble()));
                post.removeAt(i+1);
                post.removeAt(i+1);
            }

        }i--;
    }
    return post.at(0).toDouble();
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
        while(/*operadores.at(0)!="("*/!operadores.at(0).contains("("))
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

void MainWindow::metodoChancho()
{
    this->operadores.clear();
    this->postfijo.clear();
    if(this->ui->expression->text().trimmed()=="")return;
    else
        this->infijo=this->ui->expression->text().replace(" ","");
    this->postfijo.clear();
    this->ui->expression->clear();
    if(infijo.count("(")!=infijo.count(")"))
    {
        this->ui->expression->setText("Missing Parentesis");
        return;
    }
    infijo="("+infijo+")";
    infijo=infijo.toUpper().replace("SEN","sen").replace("COS","cos").replace("TAN","tan");
    infijo=reemplazarVars(infijo);
    QString currentToken;
    int flag;
    while(infijo.length()!=0)
    {
        currentToken=infijo.at(0);

        infijo.remove(0,1);
        if(currentToken==")")
        {
            meterOperador(")");
            continue;
        }
        if(currentToken.at(0).isDigit() || currentToken.at(0)=='.')
            flag=DIGIT;
        else
            flag=OPERADOR;//Luego se validara si es valido o no, aki supongamos k si

        for(;;)
        {
            if(infijo.length()==0)break;
            if((flag==DIGIT && (infijo.at(0).isDigit() || infijo.at(0)=='.'))){//Tokeniza numeros
                currentToken.append(infijo.at(0));
                infijo.remove(0,1);
            }
            else if(flag==OPERADOR && !infijo.at(0).isDigit())
            {
                if(infijo.at(0)!='('){
                    if(infijo.indexOf("cos")==0 || infijo.indexOf("sen")==0)
                        break;
                    else{
                        currentToken.append(infijo.at(0));
                        infijo.remove(0,1);
                    }
                }else
                {
                    break;
                }
            }else{

                break;
            }

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
            this->ui->expression->setText("Sintax Error!!");
            return;
        }
    }

    this->ui->expression->clear();//esto muestra la evaluacion postfija
    //for(int k=this->postfijo.count()-1;k>=0;k--)
      //  this->ui->expression->setText(this->ui->expression->text().append(postfijo.at(k)+" "));

    QString res=QString::number(this->evaluarPostFijo(postfijo));

    if(res=="inf")
        this->ui->expression->setText("Division por cero");
    else
        this->ui->expression->setText(res);
}

void MainWindow::define(QString t)
{
    QString rest=t.mid(t.indexOf(" ")).replace(" ","").toUpper();
    if(rest.count()<3)
    {
        this->ui->expression->setText("Sintax Error!!");
        return;
    }
    QChar var=rest.at(0);rest=rest.mid(1);
    if(var=='X')
    {
        this->ui->expression->setText("Variable reservada");
        return;
    }
    if(var<'A' || var>'Z')
    {
        this->ui->expression->setText("No es una variable");
        return;
    }
    QChar c=rest.at(0);rest=rest.mid(1);
    if(c!='=')this->ui->expression->setText("Sintax Error!!");
    this->ui->expression->setText(rest);
    this->metodoChancho();
    double val=this->ui->expression->text().toDouble();
    this->variables[var.toAscii()]=val;
    this->ui->expression->setText(QString("Done!! ")+var+QString(" = ")+QString::number(val));

    this->updateVars();

}

void MainWindow::updateVars()
{
    QList<char> ke=this->variables.keys();
    this->ui->vars->clear();
    for(int i=0;i<ke.count();i++)
    {
        if(variables[ke.at(i)]!=NOT_USED)
            this->ui->vars->addItem(QString(ke.at(i))+QString(" = ")+QString::number(variables[ke.at(i)]));
    }
}

QString MainWindow::reemplazarVars(QString p)
{
    int i=0;
    while(i<p.count())
    {
        QChar a=p.at(i);
        if(a>='A' && a<='Z' && a!='X')
        {
            if(variables[a.toAscii()]!=NOT_USED)
            {
                p.insert(i+1,QString::number(variables[a.toAscii()]));
                p.remove(i,1);
            }else
            {
                this->ui->expression->setText("Variable no definida!");
                break;
            }
        }
        i++;
    }
    return p;
}








void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,"About",
                       "Aplicacion creada por Néstor A. Bermúdez.\n\nProyecto de Estructura de Datos\n"
                       "Catedratico: Ing. Rogger Vasquez\n"
                       "Ingeniería en Sistemas Computacionales\n"
                       "Calculadora y Graficadora\n"
                       "Uso de QProcess para separar tareas\n\n"
                       "Sugerencias o comentarios a: nestor.bermudez@unitec.edu");
}
