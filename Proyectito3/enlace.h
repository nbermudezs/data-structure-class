#ifndef ENLACE_H
#define ENLACE_H
#include <QGraphicsLineItem>
#include <QPen>
#include <QDebug>

class NodoItem;
class Enlace: public QGraphicsLineItem
{

public:
    Enlace(NodoItem *nodo_origen, NodoItem *nodo_destino,QString peso);
    ~Enlace();

    NodoItem *getNodoOrigen() const { return this->nodo_origen; }
    NodoItem *getNodoDestino() const { return this->nodo_destino; }

    void setColor(const QColor &color);
    void setPeso(QString t){peso=t;}
    QColor color() const;

    void updatePosicion();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    NodoItem *nodo_origen;
    NodoItem *nodo_destino;
    QString peso;

};

#endif // ENLACE_H
