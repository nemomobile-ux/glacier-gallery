#include "roratewidget.h"

#include <QImage>
#include <QImageReader>

RorateWidget::RorateWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_source("")
{

}

void RorateWidget::paint(QPainter *painter)
{

}

void RorateWidget::setSource(QString source)
{
    QImageReader read;
    read.setFileName(source);
    if(read.error()) {
        qWarning() << "Wrong source";
        return;
    }

    if(source != m_source) {
        m_source = source;
        emit sourceChanged();

        update();
    }
}
