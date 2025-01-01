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
#include <QMimeDatabase>
#include <QStandardPaths>

GalleryModel::GalleryModel(QObject* parent)
    : QAbstractListModel { parent }
    , m_loading(false)
    , m_error(false)
    , m_filter(FilterMode::AllFiles)
    , m_sortMode(SortMode::SortByTime)
    , m_fileSystemWatcher(new QFileSystemWatcher)
{
    m_hash.insert(Qt::UserRole, QByteArray("url"));
    formatMimeTypes();

    connect(this, &GalleryModel::urlsChanged, this, &GalleryModel::onUrlsChanged);
    connect(m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &GalleryModel::onFileSystemChanged);
    connect(m_fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &GalleryModel::onFileSystemChanged);

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

    if (role == Qt::UserRole) {
        return m_files.at(index.row());
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

    foreach (const QString& dirString, m_urls) {
        QDir dir(dirString);
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        switch (m_sortMode) {
        case SortByName:
            dir.setSorting(QDir::Name);
            break;
        case SortByTime:
            dir.setSorting(QDir::Time);
            break;
        case SortBySize:
            dir.setSorting(QDir::Size);
            break;
        case SortByType:
            dir.setSorting(QDir::Type);
            break;
        default:
            dir.setSorting(QDir::Unsorted);
            break;
        }

        QFileInfoList filelistinfo = dir.entryInfoList();
        foreach (const QFileInfo& fileinfo, filelistinfo) {
            if (m_mimeTypes.contains(db.mimeTypeForFile(fileinfo.absoluteFilePath()).name())) {
                m_files.append(fileinfo.absoluteFilePath());
            }
        }
    }
    endResetModel();
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
    m_mimeTypes << "inode/directory";

    for (const QMimeType& mime : std::as_const(mimeList)) {
        if (m_filter == FilterMode::AllFiles) {
            if (mime.name().startsWith(QStringLiteral("image/")) || mime.name().startsWith(QStringLiteral("video/"))) {
                m_mimeTypes << mime.name();
            }
        } else if (m_filter == FilterMode::OnlyImages) {
            if (mime.name().startsWith(QStringLiteral("image/"))) {
                m_mimeTypes << mime.name();
            }
        } else if (m_filter == FilterMode::OnlyVideo) {
            if (mime.name().startsWith(QStringLiteral("video/"))) {
                m_mimeTypes << mime.name();
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
