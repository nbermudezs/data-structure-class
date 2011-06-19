#include "enlace.h"
#include "nodoitem.h"

Enlace::Enlace(NodoItem *nodo_origen, NodoItem *nodo_destino, QString peso)
{
    this->nodo_origen = nodo_origen;
    this->nodo_destino = nodo_destino;
    this->peso = peso;

       nodo_origen->addLink(this);
       nodo_destino->addLink(this);

       setFlags(QGraphicsItem::ItemIsSelectable);
       setZValue(-1);
       setColor(Qt::darkRed);
       this->updatePosicion();
}

Enlace::~Enlace()
{
    nodo_origen->removeLink(this);
    nodo_destino->removeLink(this);
}

void Enlace::setColor(const QColor &color)
{
    setPen(QPen(color, 1.0));
}

QColor Enlace::color() const
{
    return pen().color();
}

void Enlace::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::red);

    if (option->state & QStyle::State_Selected) {
        painter->setPen(Qt::blue);
        painter->setBrush(Qt::blue);

    }
    QLineF centerLine(this->getNodoOrigen()->pos(),
                      this->getNodoDestino()->pos());
    this->setLine(centerLine);
    painter->drawText((this->getNodoOrigen()->pos()+
                      this->getNodoDestino()->pos())/2,peso);
    painter->drawLine(this->line());

}

void Enlace::updatePosicion()
{
    setLine(QLineF(this->nodo_origen->pos(), this->nodo_destino->pos()));
}
