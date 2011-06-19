#include "enlace.h"
#include "NodoItem.h"

NodoItem::NodoItem()
{
    myTextColor = Qt::darkGreen;
    myOutlineColor = Qt::darkBlue;
    myBackgroundColor = Qt::white;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);

}

NodoItem::~NodoItem()
{
    foreach (Enlace *link, myLinks)
           delete link;

}

void NodoItem::setText(const QString &text)
{
    prepareGeometryChange();
    myText = text;
    update();
}

QString NodoItem::text() const
{
    return this->myText;
}

void NodoItem::setTextColor(const QColor &color)
{
    myTextColor = color;
    update();
}

QColor NodoItem::textColor() const
{
     return this->myTextColor;

}

void NodoItem::setOutlineColor(const QColor &color)
{
    myOutlineColor = color;
    update();
}

QColor NodoItem::outlineColor() const
{
    return this->myOutlineColor;
}

void NodoItem::setBackgroundColor(const QColor &color)
{
    myBackgroundColor = color;
    update();
}

QColor NodoItem::backgroundColor() const
{
    return this->myBackgroundColor;
}

void NodoItem::addLink(Enlace *link)
{
    myLinks.append(link);
}

void NodoItem::removeLink(Enlace *link)
{
    int val= myLinks.indexOf(link,0);
    if (val!=-1)
           myLinks.removeAt(val);


}

QRectF NodoItem::outlineRect() const
{
    const int Padding = 10;

    QFontMetricsF metrics = qApp->font();
    QRectF rect = metrics.boundingRect(myText);

    rect.adjust(-Padding, -Padding, +Padding, +Padding);

    rect.translate(-rect.center());
    return rect;
}

QRectF NodoItem::boundingRect() const
{
    const int Margin = 1;
    return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}

QPainterPath NodoItem::shape() const
{
    QRectF rect = outlineRect();

    QPainterPath path;
    path.addRoundRect(rect, roundness(rect.width()),
                      roundness(rect.height()));
    return path;
}

void NodoItem::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * )
{
    QPen pen(myOutlineColor);
    pen.setBrush(Qt::blue);
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }

    painter->setPen(pen);


    painter->setBrush(myBackgroundColor);

    QRectF rect = outlineRect();
    painter->drawRoundRect(rect, roundness(rect.width()),
                           roundness(rect.height()));

    painter->setPen(myTextColor);
    painter->drawText(rect, Qt::AlignCenter, myText);
}


int NodoItem::roundness(double size) const
{
    const int Diameter = 23;
    return 100 * Diameter / int(size);
}

QVariant NodoItem::itemChange(GraphicsItemChange change,
                          const QVariant &value)
{
    if (change == ItemPositionHasChanged) {
        foreach (Enlace *link, myLinks)
        {
            link->updatePosicion();

        }
    }
    return QGraphicsItem::itemChange(change, value);
}




