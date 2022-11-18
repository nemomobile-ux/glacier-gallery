#include "roratewidget.h"

#include <QImage>
#include <QImageReader>

RorateWidget::RorateWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_source("")
    , m_angle(0)
{
}

void RorateWidget::paint(QPainter *painter)
{
    QSizeF itemSize = size();
    QRectF target(0, 0, itemSize.width(), itemSize.height());

    QImage source(m_source);
    source.transformed(QMatrix().rotate(m_angle));

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

void RorateWidget::setAngle(float angle)
{
    if(angle >= 0 && angle <= 360 && angle != m_angle) {
        m_angle = angle;
        update();
    }
}
