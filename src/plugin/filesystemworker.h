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

#ifndef FILESYSTEMWORKER_H
#define FILESYSTEMWORKER_H

#include <QDirIterator>
#include <QMimeType>
#include <QMutex>
#include <QObject>

struct MediaFile {
    QString path = "";
    bool isValid = false;
    QMimeType mimeType;
    uint width = -1;
    uint height = -1;
    QDateTime modified;
    QDateTime created;
    uint size = -1;
};

class FileSystemWorker : public QObject {
    Q_OBJECT
public:
    explicit FileSystemWorker(QStringList dirList, QStringList suffixes, QObject* parent = nullptr);

    bool busy() { return m_busy; }

    void start();
    void stop();

signals:
    void foundFile(MediaFile file);

private:
    QMutex m_mutex;
    QStringList m_dirs;
    QStringList m_suffixes;
    bool m_busy;
};

#endif // FILESYSTEMWORKER_H
