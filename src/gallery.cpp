/* Copyright (C) 2012 John Brooks <john.brooks@dereferenced.net>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "gallery.h"
#include <QQuickView>
#include <QtQml>
#include <QGuiApplication>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QQuickItem>
#include <QImageReader>

Gallery::Gallery(QObject *parent)
    : QObject(parent)
{
}

void Gallery::acquireVideoResources()
{
    qDebug() << Q_FUNC_INFO;

    resources->addResource(ResourcePolicy::VideoPlaybackType);

    resources->deleteResource(ResourcePolicy::AudioPlaybackType);
    ResourcePolicy::AudioResource *audio = new ResourcePolicy::AudioResource("player");
    audio->setProcessID(QGuiApplication::applicationPid());
    audio->setStreamTag("media.name", "*");
    resources->addResourceObject(audio);

    resources->update();
    resources->acquire();
}

void Gallery::releaseVideoResources()
{
    qDebug() << Q_FUNC_INFO;
    resources->release();
}

void Gallery::resourcesGranted()
{
    qDebug() << Q_FUNC_INFO;
}

void Gallery::resourcesDenied()
{
    qDebug() << Q_FUNC_INFO;
}

void Gallery::lostResources()
{
    qDebug() << Q_FUNC_INFO;
}

int Gallery::isVideo(QString fileUrl)
{
    //RETURN VALUES
    //-1: ERROR, 0: IMAGE, 1: VIDEO
    QFileInfo testFile(fileUrl);
    if (testFile.exists())
    {
        QImageReader reader(fileUrl);
        QByteArray format = reader.format();
        if (format.isNull() && reader.error() == QImageReader::UnsupportedFormatError)
        {
            //we assume it's a video
            return 1;
        }
        else
        {
            return 0;
        }
    }
    qDebug() << fileUrl << " existis" << testFile.exists();
    return -1;
}
