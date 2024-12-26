#ifndef GALLERYMODEL_H
#define GALLERYMODEL_H

#include <QAbstractListModel>

class GalleryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList sortProperties READ sortProperties WRITE setSortProperties NOTIFY sortPropertiesChanged FINAL)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged FINAL)

public:
    explicit GalleryModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const { return m_hash; }

    QStringList sortProperties() const;
    void setSortProperties(const QStringList &newSortProperties);

    bool loading() const;

    bool error() const;

signals:
    void sortPropertiesChanged();

    void loadingChanged();

    void errorChanged();

private:
    QHash<int, QByteArray> m_hash;
    QStringList m_sortProperties;
    bool m_loading;
    bool m_error;
};

#endif // GALLERYMODEL_H
