/*
 * Copyright (C) 2024-2025 Chupligin Sergey <neochapay@gmail.com>
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

#include "gallerymodel.h"

#include <QDir>
#include <QDirIterator>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <QThread>

GalleryModel::GalleryModel(QObject* parent)
    : QAbstractListModel { parent }
    , m_loading(false)
    , m_error(false)
    , m_filter(FilterMode::AllFiles)
    , m_sortMode(SortMode::SortByTime)
    , m_fileSystemWatcher(new QFileSystemWatcher)
    , m_work(new FileSystemWorker())
{
    m_hash.insert(Qt::UserRole, QByteArray("url"));
    m_hash.insert(Qt::UserRole + 1, QByteArray("mimeType"));
    m_hash.insert(Qt::UserRole + 2, QByteArray("width"));
    m_hash.insert(Qt::UserRole + 3, QByteArray("height"));
    m_hash.insert(Qt::UserRole + 4, QByteArray("modified"));
    m_hash.insert(Qt::UserRole + 5, QByteArray("created"));
    m_hash.insert(Qt::UserRole + 6, QByteArray("fileSize"));
    formatMimeTypes();

    connect(this, &GalleryModel::urlsChanged, this, &GalleryModel::onUrlsChanged);
    connect(m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &GalleryModel::onFileSystemChanged);
    connect(m_fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &GalleryModel::onFileSystemChanged);

    QThread* scanTread = new QThread;
    connect(m_work, &FileSystemWorker::foundFile, this, &GalleryModel::appendFiles);
    connect(m_work ,&FileSystemWorker::busyChanged, [=](){
        if(m_work->busy() != m_loading) {
            m_loading = m_work->busy();
            emit loadingChanged();
        }
    });

    m_work->moveToThread(scanTread);
    scanTread->start();

    formatFileList();
}

GalleryModel::~GalleryModel()
{
    if (m_fileSystemWatcher != nullptr) {
        delete m_fileSystemWatcher;
    }
}

int GalleryModel::rowCount(const QModelIndex& parent) const
{
    return m_files.count();
}

QVariant GalleryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    MediaFile file = m_files.at(index.row());
    if (role == Qt::UserRole) {
        return file.path;
    } else if (role == Qt::UserRole+1) {
        return file.mimeType.name();
    } else if (role == Qt::UserRole+2) {
        return file.width;
    } else if (role == Qt::UserRole+3) {
        return file.height;
    } else if (role == Qt::UserRole+4) {
        return file.modified;
    } else if (role == Qt::UserRole+5) {
        return file.created;
    } else if (role == Qt::UserRole+6) {
        return file.size;
    }


    return QVariant();
}

bool GalleryModel::loading() const
{
    return m_loading;
}

bool GalleryModel::error() const
{
    return m_error;
}

GalleryModel::FilterMode GalleryModel::filter() const
{
    return m_filter;
}

void GalleryModel::setFilter(FilterMode newFilter)
{
    if (m_filter == newFilter)
        return;
    m_filter = newFilter;

    formatMimeTypes();
    emit filterChanged();
}

void GalleryModel::addPath(QString url)
{
    if (!m_urls.contains(url)) {
        m_urls.append(url);
        emit urlsChanged();
    }

    if (url.isEmpty()) {
        m_urls.append(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation));
        emit urlsChanged();
    }
}

void GalleryModel::removePatch(QString url)
{
    if (url.isEmpty()) {
        m_urls.clear();
        emit urlsChanged();
    } else if (m_urls.contains(url)) {
        m_urls.removeAll(url);
        emit urlsChanged();
    }
}

void GalleryModel::onUrlsChanged()
{
    m_fileSystemWatcher->removePaths(m_fileSystemWatcher->directories());
    m_fileSystemWatcher->addPaths(m_urls);
}

void GalleryModel::formatFileList()
{
    beginResetModel();
    QMimeDatabase db;

    if (m_urls.empty()) {
        addPath();
    }

    m_files.clear();
    if(m_work != nullptr) {
        m_work->stop();
    }

    QStringList suffixes;
    foreach (const QMimeType mType, m_mimeTypes) {
        foreach (QString suff, mType.suffixes()) {
            suffixes << "*." + suff;
        }
    };

    m_work->setDirs(m_urls);
    m_work->setSuffixes(suffixes);
    m_work->start();

    endResetModel();
}

void GalleryModel::appendFiles(MediaFile file)
{
    if (!file.isValid) {
        return;
    }

    beginInsertRows(QModelIndex(), m_files.count(), m_files.count());
    m_files.push_back(file);
    endInsertRows();
}

void GalleryModel::onFileSystemChanged(QString path)
{
    qDebug() << Q_FUNC_INFO << path;
}

void GalleryModel::formatMimeTypes()
{
    QMimeDatabase db;
    QList<QMimeType> mimeList = db.allMimeTypes();

    m_mimeTypes.clear();

    for (const QMimeType& mime : std::as_const(mimeList)) {
        if (m_filter == FilterMode::AllFiles) {
            if (mime.name().startsWith(QStringLiteral("image/")) || mime.name().startsWith(QStringLiteral("video/"))) {
                m_mimeTypes << mime;
            }
        } else if (m_filter == FilterMode::OnlyImages) {
            if (mime.name().startsWith(QStringLiteral("image/"))) {
                m_mimeTypes << mime;
            }
        } else if (m_filter == FilterMode::OnlyVideo) {
            if (mime.name().startsWith(QStringLiteral("video/"))) {
                m_mimeTypes << mime;
            }
        }
    }
    formatFileList();
}

GalleryModel::SortMode GalleryModel::sortMode() const
{
    return m_sortMode;
}

void GalleryModel::setSortMode(const GalleryModel::SortMode& newSort)
{
    if (m_sortMode == newSort)
        return;
    m_sortMode = newSort;
    emit sortModeChanged();

    formatFileList();
}

QString GalleryModel::sizeTotext(float size)
{
    QStringList list;
    list << tr("kb") << tr("mb") << tr("gb") << tr("tb");

    QStringListIterator i(list);
    QString unit("bytes");

    while (size >= 1024.0 && i.hasNext()) {
        unit = i.next();
        size /= 1024.0;
    }
    return QString().setNum(size, 'f', 2) + " " + unit;
}

QVariant GalleryModel::get(const int idx)
{
    if (idx >= m_files.size() || idx < 0) {
        return QVariant();
    }

    QMap<QString, QVariant> itemData;
    MediaFile item = m_files.at(idx);

    itemData.insert("url", item.path);
    itemData.insert("mimeType", item.mimeType.name());
    itemData.insert("width", item.width);
    itemData.insert("height", item.height);
    itemData.insert("modified", item.modified);
    itemData.insert("created", item.created);
    itemData.insert("fileSize", item.size);

    return QVariant(itemData);
}
