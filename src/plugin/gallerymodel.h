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

#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>
#include <QFileSystemWatcher>
#include <QMimeType>

class GalleryModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged FINAL)
    Q_PROPERTY(GalleryModel::FilterMode filter READ filter WRITE setFilter NOTIFY filterChanged FINAL)
    Q_PROPERTY(GalleryModel::SortMode sortMode READ sortMode WRITE setSortMode NOTIFY sortModeChanged FINAL)

public:
    enum FilterMode {
        AllFiles,
        OnlyImages,
        OnlyVideo
    };

    enum SortMode {
        SortByName = 0,
        SortByTime,
        SortBySize,
        SortByType,
        Unsorted = 255
    };
    Q_ENUMS(FilterMode)
    Q_ENUMS(SortMode)

    explicit GalleryModel(QObject* parent = nullptr);
    virtual ~GalleryModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const { return m_hash; }

    bool loading() const;
    bool error() const;

    FilterMode filter() const;
    void setFilter(FilterMode newFilter);

    void addPath(QString url = "");
    void removePatch(QString url = "");

    GalleryModel::SortMode sortMode() const;
    void setSortMode(const GalleryModel::SortMode& newSort);

    Q_INVOKABLE QString sizeTotext(float size);
    Q_INVOKABLE bool isVideo(int index);

public slots:
    QVariant get(const int idx);

signals:
    void sortPropertiesChanged();
    void loadingChanged();
    void errorChanged();
    void filterChanged();
    void urlsChanged();
    void sortModeChanged();

private slots:
    void onUrlsChanged();
    void onFileSystemChanged(QString path);
    void formatFileList();
    void appendFiles(QString path);

private:
    QHash<int, QByteArray> m_hash;
    bool m_loading;
    bool m_error;
    GalleryModel::FilterMode m_filter;
    GalleryModel::SortMode m_sortMode;

    QList<QMimeType> m_mimeTypes;
    QStringList m_urls;
    QList<QString> m_files;

    QFileSystemWatcher* m_fileSystemWatcher;

    void formatMimeTypes();
};

#endif // GALLERYMODEL_H
