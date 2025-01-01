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

FileSystemWorker::FileSystemWorker(QStringList dirList, QStringList suffixes, QObject *parent)
    : QObject{parent}
    , m_dirs(dirList)
    , m_suffixes(suffixes)
    , m_busy(false)
{
}

void FileSystemWorker::start()
{
    m_busy = true;
    foreach (const QString& dirString, m_dirs) {
        QDirIterator it(dirString,  m_suffixes, QDir::Files,  QDirIterator::Subdirectories);
        while (it.hasNext()) {
            emit foundFile(it.next());
        }
    }
    m_busy = false;
}
