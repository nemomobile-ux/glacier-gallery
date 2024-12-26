#include "gallerymodel.h"

GalleryModel::GalleryModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_loading(false)
    , m_error(true)
{
}

int GalleryModel::rowCount(const QModelIndex &parent) const
{
    return -1;
}

QVariant GalleryModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QStringList GalleryModel::sortProperties() const
{
    return m_sortProperties;
}

void GalleryModel::setSortProperties(const QStringList &newSortProperties)
{
    if (m_sortProperties == newSortProperties)
        return;
    m_sortProperties = newSortProperties;
    emit sortPropertiesChanged();
}

bool GalleryModel::loading() const
{
    return m_loading;
}

bool GalleryModel::error() const
{
    return m_error;
}
