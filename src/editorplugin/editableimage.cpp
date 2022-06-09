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
    , m_mouseButtonPressed(false)
    , m_cropperVisible(false)
    , m_cropperRect(QRectF())
    , m_topSelectionDot(QRectF())
    , m_leftSelectionDot(QRectF())
    , m_rightSelectionDot(QRectF())
    , m_bottomSelectionDot(QRectF())
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
}

void EditableImage::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

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

    /*
Cropper
*/
    if(m_cropperVisible) {
        /*If cropper not run before set center and width*/
        if(m_cropperRect == QRectF()) {
            m_cropperRect.setX(offsetX);
            m_cropperRect.setY(offsetY);
            m_cropperRect.setWidth(drawImage.width());
            m_cropperRect.setHeight(drawImage.height());
        }
        // Need create api for get currrent theme color
        //                        accentColor
        //                            V
        QColor selectorColor("#0091e5");
        painter->setBrush(QBrush(selectorColor));
        // Selection dots
        // top dot
        m_topSelectionDot.setRect(m_cropperRect.x()+m_cropperRect.width()/2-10,
                                  m_cropperRect.y()-10,
                                  20,20);
        painter->drawEllipse(m_topSelectionDot);
        // left dot
        m_leftSelectionDot.setRect(m_cropperRect.x()-10,
                                   m_cropperRect.y()+m_cropperRect.height()/2-10,
                                   20,20);
        painter->drawEllipse(m_leftSelectionDot);
        //right dot
        m_rightSelectionDot.setRect(m_cropperRect.x()+m_cropperRect.width()-13,
                                    m_cropperRect.y()+m_cropperRect.height()/2-10,
                                    20,20);
        painter->drawEllipse(m_rightSelectionDot);
        //bottom dot
        m_bottomSelectionDot.setRect(m_cropperRect.x()+m_cropperRect.width()/2-10,
                                     m_cropperRect.y()+m_cropperRect.height()-13,
                                     20,20);
        painter->drawEllipse(m_bottomSelectionDot);

        // Selection zone
        selectorColor.setAlphaF(0.2);
        painter->setBrush(QBrush(selectorColor));
        painter->drawRect(QRectF(m_cropperRect.x(),m_cropperRect.y(),m_cropperRect.width(),m_cropperRect.height()));

        // Selection border
        painter->setBrush(Qt::NoBrush);
        QPen borderPen(QColor("#0091e5"), 6, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        painter->setPen(borderPen);
        painter->drawRect(QRectF(m_cropperRect.x(),m_cropperRect.y(),m_cropperRect.width()-3,m_cropperRect.height()-3));
    }
}

void EditableImage::setSource(QString source)
{
    source = source.remove("file://");

    if(source != m_source && !source.isEmpty() && QFile::exists(source)) {
        if(m_image.load(source)) {
            m_source = source;
            emit sourceChanged();

            m_cropperRect = QRect(0,0,0,0);
            m_cropperVisible = false;

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

void EditableImage::showCropper()
{
    if(!m_cropperVisible) {
        m_cropperVisible = true;
        update();
    }
}

void EditableImage::hideCropper()
{
    if(m_cropperVisible) {
        m_cropperVisible = false;
        update();
    }
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

void EditableImage::mousePressEvent(QMouseEvent *event)
{
    m_mouseButtonPressed = true;
}

void EditableImage::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseButtonPressed = false;

    m_topSelectionDotPressed  = false;
    m_leftSelectionDotPressed  = false;
    m_rightSelectionDotPressed = false;
    m_bottomSelectionDotPressed = false;
    m_cropperRectSelected = false;
    m_newCroppSeleced = false;
}

void EditableImage::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouseButtonPressed) {
        /* if cropper wisible*/
        if(m_cropperVisible) {
            if(moveCropperRect(event)) {
                update();
            }
        }
    }
}

void EditableImage::touchEvent(QTouchEvent *event)
{

}

bool EditableImage::moveCropperRect(QMouseEvent *event)
{
    /*
check points
*/
    if(!m_topSelectionDotPressed &&
            !m_leftSelectionDotPressed &&
            !m_rightSelectionDotPressed &&
            !m_bottomSelectionDotPressed &&
            !m_cropperRectSelected &&
            !m_newCroppSeleced) {
        if(m_topSelectionDot.contains(event->pos())) {
            m_topSelectionDotPressed = true;
        } else if(m_leftSelectionDot.contains(event->pos())) {
            m_leftSelectionDotPressed = true;
        } else if(m_rightSelectionDot.contains(event->pos())) {
            m_rightSelectionDotPressed = true;
        } else if(m_bottomSelectionDot.contains(event->pos())) {
            m_bottomSelectionDotPressed = true;
        } else if(m_cropperRect.contains(event->pos())) {
            m_cropperRectSelected = true;
            m_firstTouchX = -m_cropperRect.x()+event->pos().x();
            m_firstTouchY = -m_cropperRect.y()+event->pos().y();
        } else {
            m_newCroppSeleced = true;
            m_firstTouchX = event->pos().x();
            m_firstTouchY = event->pos().y();
        }
    }

    if(m_topSelectionDotPressed) {
        m_cropperRect.setY(event->pos().y());
        return true;
    }

    if(m_leftSelectionDotPressed) {
        m_cropperRect.setX(event->pos().x());
        return true;
    }

    if(m_rightSelectionDotPressed) {
        m_cropperRect.setWidth(event->pos().x()-m_cropperRect.x());
        return true;
    }

    if(m_bottomSelectionDotPressed) {
        m_cropperRect.setHeight(event->pos().y()-m_cropperRect.y());
        return true;
    }

    if(m_cropperRectSelected) {
        m_cropperRect.setRect(event->pos().x()-m_firstTouchX
                              , event->pos().y()-m_firstTouchY
                              , m_cropperRect.width()
                              , m_cropperRect.height());
        return true;
    }

    if(m_newCroppSeleced) {
        m_cropperRect.setRect(m_firstTouchX
                              , m_firstTouchY
                              , event->pos().x() - m_firstTouchX
                              , event->pos().y() - m_firstTouchY);
        return true;
    }

    return false;
}
