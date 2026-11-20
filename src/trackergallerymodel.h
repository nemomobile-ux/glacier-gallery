/*
 * Copyright (C) 2026 Chupligin Sergey <neochapay@gmail.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
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
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#ifndef TRACKERGALLERYMODEL_H
#define TRACKERGALLERYMODEL_H

#include <QAbstractListModel>
#include <QVariantMap>
#include <QVector>

class TrackerGalleryModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(bool error READ error NOTIFY errorChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(QString sortProperty READ sortProperty WRITE setSortProperty NOTIFY sortPropertyChanged)
    Q_PROPERTY(QString filterMode READ filterMode WRITE setFilterMode NOTIFY filterModeChanged)

public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        FileNameRole,
        FileSizeRole,
        LastModifiedRole,
        MimeTypeRole,
        OrientationRole,
        LatitudeRole,
        LongitudeRole,
        AltitudeRole,
        DateTakenRole,
        CreatedRole,
        ExposureTimeRole,
        FNumberRole,
        FlashEnabledRole,
        FocalLengthRole,
        MeteringModeRole,
        WhiteBalanceRole,
        CameraManufacturerRole,
        CameraModelRole,
        DurationRole,
        FrameRateRole
    };
    Q_ENUM(Roles)

    explicit TrackerGalleryModel(QObject* parent = nullptr);
    ~TrackerGalleryModel() override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool loading() const { return m_loading; }
    bool error() const { return m_error; }

    QString sortProperty() const { return m_sortProperty; }
    void setSortProperty(const QString& property);

    QString filterMode() const { return m_filterMode; }
    void setFilterMode(const QString& mode);

    Q_INVOKABLE QVariantMap get(int index) const;
    Q_INVOKABLE bool isVideo(int index) const;

public slots:
    void refresh();

signals:
    void loadingChanged();
    void errorChanged();
    void countChanged();
    void sortPropertyChanged();
    void filterModeChanged();

private:
    struct Item {
        QVariantMap values;
    };

    void setLoading(bool loading);
    void setError(bool error);
    void applyResults(const QVector<QVariantMap>& items, bool ok);
    QString buildQuery() const;

    QVector<Item> m_items;
    bool m_loading = false;
    bool m_error = false;
    QString m_sortProperty;
    QString m_filterMode = QStringLiteral("all");
    quint64 m_queryGeneration = 0;
};

#endif // TRACKERGALLERYMODEL_H
