#include "calculos.h"

Calculos::Calculos()
{
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

double Calculos::evaluar(double val)
{
    QString nueva=this->postFijo.toUpper();
    nueva.replace("X",QString::number(val)).replace("COS","cos").replace("SEN","sen");
    QStringList p=nueva.split(" ",QString::SkipEmptyParts);
    return evaluarPostFijo(p);

}

double Calculos::evaluarPostFijo(QStringList post)
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

int Calculos::existeOperador(QString op)
{
    for(int i=0;i<this->jerarquiaOperadores.count();i++)
    {
        for(int j=0;j<this->jerarquiaOperadores.at(i).operadores.count();j++)
            if(jerarquiaOperadores.at(i).operadores.at(j)==op)
                return jerarquiaOperadores.at(i).nivel;
    }
    return -1;
}

QString Calculos::toPost(QString infijo)
{
        this->operadores.clear();
        this->postfijo.clear();
        infijo="("+infijo+")";
        infijo=infijo.toUpper().replace("SEN","sen").replace("COS","cos");
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
            if(currentToken=="X")
            {
                postfijo.push_front(currentToken);
                continue;
            }
            if(currentToken.at(0).isDigit() || currentToken.at(0)=='.')
                flag=DIGIT;
            else
                flag=OPERADOR;

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
                            if(infijo.indexOf("X")==0)
                            {
                                this->postfijo.push_front("X");
                                infijo.remove(0,1);
                                break;
                            }
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
                return "";
            }
        }
        QString al="";
        for(int i=0;i<postfijo.count();i++)
            al.append(postfijo.at(i)+" ");
        qDebug()<<al<<"...............................";
        return al;


}

void Calculos::meterOperador(QString op)
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
