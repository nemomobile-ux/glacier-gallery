/*
 * Copyright (C) 2022 Chupligin Sergey <neochapay@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef EDITABLEIMAGE_H
#define EDITABLEIMAGE_H

#include <QImage>
#include <QObject>
#include <QQuickPaintedItem>

class EditableImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit EditableImage(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);


    QString source() {return m_source;}
    void setSource(QString source);

    Q_INVOKABLE void rotateLeft();
    Q_INVOKABLE void rotateRight();
    Q_INVOKABLE void flipHorizontaly();
    Q_INVOKABLE void flipVetricaly();

    Q_INVOKABLE void save(bool replace = false);

signals:
    void sourceChanged();

private:
    QString m_source;
    QImage m_image;
};

#endif // EDITABLEIMAGE_H
