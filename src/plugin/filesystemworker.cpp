/*
 * Copyright (C) 2025 Chupligin Sergey <neochapay@gmail.com>
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

#include "filesystemworker.h"

#include <QDir>
#include <QDirIterator>
#include <QImageReader>
#include <QMimeDatabase>

FileSystemWorker::FileSystemWorker(QStringList dirList, QStringList suffixes, QObject* parent)
    : QObject { parent }
    , m_dirs(dirList)
    , m_suffixes(suffixes)
    , m_busy(false)
{
}

void FileSystemWorker::start()
{
    if (m_busy) {
        qWarning() << "Stop before run again!";
        return;
    }
    QMimeDatabase db;

    m_busy = true;
    m_mutex.lock();
    foreach (const QString& dirString, m_dirs) {
        if (m_mutex.tryLock()) {
            break;
        }
        QDirIterator it(dirString, m_suffixes, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFileInfo fInfo(filePath);

            MediaFile file;
            file.path = filePath;
            file.created = fInfo.birthTime();
            file.modified = fInfo.lastModified();
            file.mimeType = db.mimeTypeForFile(fInfo.absoluteFilePath());
            file.size = fInfo.size();

            if (file.mimeType.name().startsWith("image/")) {
                QImageReader reader(file.path);
                QSize size = reader.size();
                if (size.isValid()) {
                    file.width = size.width();
                    file.height = size.height();
                } else {
                    QImage image = reader.read();
                    file.width = image.width();
                    file.height = image.height();
                }
            } else {
                qWarning() << "Unsuported mime " << file.mimeType.name();
            }
            file.isValid = file.height > 0 && file.width > 0;
            emit foundFile(file);
        }
    }
    m_mutex.unlock();
    m_busy = false;
}

void FileSystemWorker::stop()
{
    m_mutex.unlock();
    m_busy = false;
}
