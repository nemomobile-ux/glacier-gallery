#ifndef RORATEWIDGET_H
#define RORATEWIDGET_H

#include <QQuickPaintedItem>

class RorateWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit RorateWidget(QQuickItem *parent = 0);
    void paint(QPainter *painter);

    QString source() {return m_source;}
    void setSource(QString source);

signals:
    void sourceChanged();

private:
    QString m_source;
};

#endif // RORATEWIDGET_H
