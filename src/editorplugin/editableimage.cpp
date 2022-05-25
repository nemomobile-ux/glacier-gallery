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

#include "editableimage.h"

#include <QFile>
#include <QPainter>

EditableImage::EditableImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_source("")
{

}

void EditableImage::paint(QPainter *painter)
{
    QSizeF itemSize = size();
    QRectF target(0, 0, itemSize.width(), itemSize.height());
    /*
Draw background
*/
    painter->setBrush(QBrush("000000"));
    painter->drawRect(target);
    /*
Calculate image size and offset for fit image
*/
    QImage drawImage = m_image.scaled(itemSize.toSize(), Qt::AspectRatioMode::KeepAspectRatio);
    int offsetX, offsetY;
    offsetX = (itemSize.width() - drawImage.width())/2;
    offsetY = (itemSize.height() - drawImage.height())/2;

    if(offsetX < 0) {
        offsetX = 0;
    }

    if(offsetY < 0) {
        offsetY = 0;
    }

    painter->drawImage(offsetX, offsetY,drawImage);
}

void EditableImage::setSource(QString source)
{
    source = source.remove("file://");

    if(source != m_source && !source.isEmpty() && QFile::exists(source)) {
        if(m_image.load(source)) {
            m_source = source;
            emit sourceChanged();

            update();
        } else {
            qWarning() << "Wrong image" << source;
        }
    } else {
        qWarning() << "Wrong image path" << source;
    }
}

void EditableImage::rotateLeft()
{
    if(m_image.isNull()) {
        return;
    }
    m_image = m_image.transformed(QTransform().rotate(-90.0));
    update();
}

void EditableImage::rotateRight()
{
    if(m_image.isNull()) {
        return;
    }

    m_image = m_image.transformed(QTransform().rotate(90.0));
    update();
}

void EditableImage::flipHorizontaly()
{
    if(m_image.isNull()) {
        return;
    }
    m_image = m_image.mirrored(false, true);
    update();
}

void EditableImage::flipVetricaly()
{
    if(m_image.isNull()) {
        return;
    }
    m_image = m_image.mirrored(true, false);
    update();
}

void EditableImage::save(bool replace)
{
    QString fileName = m_source;
    if(!replace) {
        int idx = 1;
        QString croppedFileName = fileName.split(".", Qt::SkipEmptyParts).at(0);
        QString extFileName = fileName.split(".", Qt::SkipEmptyParts).at(1);

        while(QFile::exists(fileName)) {
            fileName = QString("%1_%2.%3" ).arg(croppedFileName).arg(idx++).arg(extFileName);
        }
    }

    m_image.save(fileName);
    m_source = fileName;
    emit sourceChanged();
}
