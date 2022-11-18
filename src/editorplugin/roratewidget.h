#ifndef RORATEWIDGET_H
#define RORATEWIDGET_H

#include <QQuickPaintedItem>

class RorateWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(float angle READ angle WRITE setAngle NOTIFY angleChanged)

public:
    explicit RorateWidget(QQuickItem *parent = 0);
    void paint(QPainter *painter);

    QString source() {return m_source;}
    void setSource(QString source);

    float angle() {return m_angle;}
    void setAngle(float angle);

signals:
    void sourceChanged();
    void angleChanged();

private:
    QString m_source;
    float m_angle;
};

#endif // RORATEWIDGET_H
