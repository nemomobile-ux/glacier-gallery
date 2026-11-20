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

#include <gio/gio.h>
#include <libtracker-sparql/tracker-sparql.h>

#include "trackergallerymodel.h"

#include <QDateTime>
#include <QDebug>
#include <QMetaObject>
#include <QUrl>
#include <QtGlobal>

#include <thread>

namespace {

QString cursorString(TrackerSparqlCursor* cursor, int col)
{
    if (tracker_sparql_cursor_is_bound(cursor, col) == FALSE)
        return QString();
    const gchar* value = tracker_sparql_cursor_get_string(cursor, col, nullptr);
    return value ? QString::fromUtf8(value) : QString();
}

QVariant cursorValue(TrackerSparqlCursor* cursor, int col)
{
    if (tracker_sparql_cursor_is_bound(cursor, col) == FALSE)
        return QVariant();

    TrackerSparqlValueType type = tracker_sparql_cursor_get_value_type(cursor, col);
    switch (type) {
    case TRACKER_SPARQL_VALUE_TYPE_INTEGER:
        return QVariant::fromValue(tracker_sparql_cursor_get_integer(cursor, col));
    case TRACKER_SPARQL_VALUE_TYPE_DOUBLE:
        return QVariant(tracker_sparql_cursor_get_double(cursor, col));
    case TRACKER_SPARQL_VALUE_TYPE_BOOLEAN:
        return QVariant(tracker_sparql_cursor_get_boolean(cursor, col) != FALSE);
    case TRACKER_SPARQL_VALUE_TYPE_DATETIME: {
        const gchar* s = tracker_sparql_cursor_get_string(cursor, col, nullptr);
        if (!s)
            return QVariant();
        QDateTime dt = QDateTime::fromString(QString::fromUtf8(s), Qt::ISODate);
        if (!dt.isValid())
            dt = QDateTime::fromString(QString::fromUtf8(s), Qt::ISODateWithMs);
        return dt.isValid() ? QVariant(dt) : QVariant(QString::fromUtf8(s));
    }
    case TRACKER_SPARQL_VALUE_TYPE_STRING:
    case TRACKER_SPARQL_VALUE_TYPE_URI:
    case TRACKER_SPARQL_VALUE_TYPE_BLANK_NODE:
    case TRACKER_SPARQL_VALUE_TYPE_UNBOUND:
    default: {
        const gchar* s = tracker_sparql_cursor_get_string(cursor, col, nullptr);
        return s ? QVariant(QString::fromUtf8(s)) : QVariant();
    }
    }
}

QVector<QVariantMap> runQuery(const QString& sparql, bool& ok)
{
    QVector<QVariantMap> results;
    ok = false;

    static const char* busNames[] = {
        "org.freedesktop.Tracker3.Miner.Files",
        "org.freedesktop.LocalSearch3",
        "org.freedesktop.Tracker3.Miner.Files.Control",
        nullptr
    };

    GError* error = nullptr;
    TrackerSparqlConnection* connection = nullptr;
    for (int i = 0; busNames[i] != nullptr; ++i) {
        g_clear_error(&error);
        connection = tracker_sparql_connection_bus_new(busNames[i], nullptr, nullptr, &error);
        if (connection)
            break;
    }
    if (!connection) {
        if (error) {
            qWarning() << "Tracker connection failed:" << error->message;
            g_error_free(error);
        } else {
            qWarning() << "Tracker connection failed: no known Tracker miner service found";
        }
        return results;
    }

    TrackerSparqlCursor* cursor = tracker_sparql_connection_query(
        connection, sparql.toUtf8().constData(), nullptr, &error);
    if (!cursor) {
        if (error) {
            qWarning() << "Tracker query failed:" << error->message;
            g_error_free(error);
        } else {
            qWarning() << "Tracker query failed";
        }
        g_object_unref(connection);
        return results;
    }

    const char* names[] = {
        "url", "fileName", "fileSize", "lastModified", "mimeType", "orientation",
        "latitude", "longitude", "altitude", "dateTaken", "created",
        "exposureTime", "fNumber", "flashEnabled", "focalLength", "meteringMode", "whiteBalance",
        "cameraManufacturer", "cameraModel", "duration", "frameRate"
    };
    const int colCount = sizeof(names) / sizeof(names[0]);
    int cols[colCount];
    for (int i = 0; i < colCount; ++i)
        cols[i] = -1;

    int ncols = tracker_sparql_cursor_get_n_columns(cursor);
    for (int c = 0; c < ncols; ++c) {
        const char* vname = tracker_sparql_cursor_get_variable_name(cursor, c);
        if (!vname)
            continue;
        for (int i = 0; i < colCount; ++i) {
            if (qstrcmp(vname, names[i]) == 0) {
                cols[i] = c;
                break;
            }
        }
    }

    while (tracker_sparql_cursor_next(cursor, nullptr, &error)) {
        QVariantMap item;
        for (int i = 0; i < colCount; ++i) {
            if (cols[i] < 0)
                continue;
            QVariant v = cursorValue(cursor, cols[i]);
            if (!v.isValid())
                continue;
            if (qstrcmp(names[i], "url") == 0) {
                QString u = v.toString();
                item.insert(QString::fromLatin1(names[i]), QUrl(u));
            } else {
                item.insert(QString::fromLatin1(names[i]), v);
            }
        }
        if (item.contains(QStringLiteral("url")))
            results.append(item);
    }

    if (error) {
        qWarning() << "Tracker cursor error:" << error->message;
        g_error_free(error);
        g_object_unref(cursor);
        g_object_unref(connection);
        return QVector<QVariantMap>();
    }

    ok = true;
    g_object_unref(cursor);
    g_object_unref(connection);
    return results;
}

} // namespace

TrackerGalleryModel::TrackerGalleryModel(QObject* parent)
    : QAbstractListModel(parent)
{
    refresh();
}

TrackerGalleryModel::~TrackerGalleryModel() = default;

int TrackerGalleryModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.count();
}

QVariant TrackerGalleryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    const QVariantMap& v = m_items.at(index.row()).values;
    switch (role) {
    case UrlRole:
        return v.value(QStringLiteral("url"));
    case FileNameRole:
        return v.value(QStringLiteral("fileName"));
    case FileSizeRole:
        return v.value(QStringLiteral("fileSize"));
    case LastModifiedRole:
        return v.value(QStringLiteral("lastModified"));
    case MimeTypeRole:
        return v.value(QStringLiteral("mimeType"));
    case OrientationRole:
        return v.value(QStringLiteral("orientation"));
    case LatitudeRole:
        return v.value(QStringLiteral("latitude"));
    case LongitudeRole:
        return v.value(QStringLiteral("longitude"));
    case AltitudeRole:
        return v.value(QStringLiteral("altitude"));
    case DateTakenRole:
        return v.value(QStringLiteral("dateTaken"));
    case CreatedRole:
        return v.value(QStringLiteral("created"));
    case ExposureTimeRole:
        return v.value(QStringLiteral("exposureTime"));
    case FNumberRole:
        return v.value(QStringLiteral("fNumber"));
    case FlashEnabledRole:
        return v.value(QStringLiteral("flashEnabled"));
    case FocalLengthRole:
        return v.value(QStringLiteral("focalLength"));
    case MeteringModeRole:
        return v.value(QStringLiteral("meteringMode"));
    case WhiteBalanceRole:
        return v.value(QStringLiteral("whiteBalance"));
    case CameraManufacturerRole:
        return v.value(QStringLiteral("cameraManufacturer"));
    case CameraModelRole:
        return v.value(QStringLiteral("cameraModel"));
    case DurationRole:
        return v.value(QStringLiteral("duration"));
    case FrameRateRole:
        return v.value(QStringLiteral("frameRate"));
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> TrackerGalleryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[FileNameRole] = "fileName";
    roles[FileSizeRole] = "fileSize";
    roles[LastModifiedRole] = "lastModified";
    roles[MimeTypeRole] = "mimeType";
    roles[OrientationRole] = "orientation";
    roles[LatitudeRole] = "latitude";
    roles[LongitudeRole] = "longitude";
    roles[AltitudeRole] = "altitude";
    roles[DateTakenRole] = "dateTaken";
    roles[CreatedRole] = "created";
    roles[ExposureTimeRole] = "exposureTime";
    roles[FNumberRole] = "fNumber";
    roles[FlashEnabledRole] = "flashEnabled";
    roles[FocalLengthRole] = "focalLength";
    roles[MeteringModeRole] = "meteringMode";
    roles[WhiteBalanceRole] = "whiteBalance";
    roles[CameraManufacturerRole] = "cameraManufacturer";
    roles[CameraModelRole] = "cameraModel";
    roles[DurationRole] = "duration";
    roles[FrameRateRole] = "frameRate";
    return roles;
}

void TrackerGalleryModel::setSortProperty(const QString& property)
{
    if (m_sortProperty == property)
        return;
    m_sortProperty = property;
    emit sortPropertyChanged();
    refresh();
}

void TrackerGalleryModel::setFilterMode(const QString& mode)
{
    if (m_filterMode == mode)
        return;
    m_filterMode = mode;
    emit filterModeChanged();
    refresh();
}

QVariantMap TrackerGalleryModel::get(int index) const
{
    if (index < 0 || index >= m_items.count())
        return QVariantMap();
    QVariantMap map = m_items.at(index).values;
    // Provide url as string for QML code expecting strings where appropriate
    if (map.contains(QStringLiteral("url"))) {
        QUrl u = map.value(QStringLiteral("url")).toUrl();
        if (u.isValid())
            map.insert(QStringLiteral("url"), u.toString());
    }
    return map;
}

bool TrackerGalleryModel::isVideo(int index) const
{
    if (index < 0 || index >= m_items.count())
        return false;
    const QString mime = m_items.at(index).values.value(QStringLiteral("mimeType")).toString();
    return mime.startsWith(QStringLiteral("video"));
}

void TrackerGalleryModel::setLoading(bool loading)
{
    if (m_loading == loading)
        return;
    m_loading = loading;
    emit loadingChanged();
}

void TrackerGalleryModel::setError(bool error)
{
    if (m_error == error)
        return;
    m_error = error;
    emit errorChanged();
}

void TrackerGalleryModel::applyResults(const QVector<QVariantMap>& items, bool ok)
{
    beginResetModel();
    m_items.clear();
    m_items.reserve(items.size());
    for (const QVariantMap& v : items) {
        Item it;
        it.values = v;
        m_items.append(it);
    }
    endResetModel();
    setError(!ok);
    setLoading(false);
    emit countChanged();
}

QString TrackerGalleryModel::buildQuery() const
{
    QString typeFilter;
    if (m_filterMode == QStringLiteral("images")) {
        typeFilter = QStringLiteral("?r a nfo:Image .");
    } else if (m_filterMode == QStringLiteral("videos")) {
        typeFilter = QStringLiteral("?r a nfo:Video .");
    } else {
        typeFilter = QStringLiteral("{ ?r a nfo:Image } UNION { ?r a nfo:Video } .");
    }

    QString orderExpr = QStringLiteral("nfo:fileName(?r)");
    if (m_sortProperty == QStringLiteral("lastModified")) {
        orderExpr = QStringLiteral("?lastModified");
    } else if (m_sortProperty == QStringLiteral("fileName")) {
        orderExpr = QStringLiteral("nfo:fileName(?r)");
    } else if (m_sortProperty.isEmpty()) {
        orderExpr = QStringLiteral("nfo:fileName(?r)");
    }

    // Best-effort mapping of properties from Tracker ontologies.
    const QString sparql = QStringLiteral(
        "SELECT "
        "  ?url ?fileName ?fileSize ?lastModified ?mimeType ?orientation "
        "  ?latitude ?longitude ?altitude ?dateTaken ?created "
        "  ?exposureTime ?fNumber ?flashEnabled ?focalLength ?meteringMode ?whiteBalance "
        "  ?cameraManufacturer ?cameraModel ?duration ?frameRate "
        "WHERE { "
        "  %1 "
        "  OPTIONAL { ?r nie:isStoredAs ?f . ?f nie:url ?fileUrl . } "
        "  OPTIONAL { ?r nie:url ?resUrl . } "
        "  BIND(COALESCE(?fileUrl, ?resUrl) AS ?url) "
        "  OPTIONAL { ?r nfo:fileName ?fileName . } "
        "  OPTIONAL { ?r nfo:fileSize ?fileSize . } "
        "  OPTIONAL { ?r nfo:fileLastModified ?lastModified . } "
        "  OPTIONAL { ?r nie:mimeType ?mimeType . } "
        "  OPTIONAL { ?r nfo:orientation ?orientation . } "
        "  OPTIONAL { ?r slo:latitude ?latitude . } "
        "  OPTIONAL { ?r slo:longitude ?longitude . } "
        "  OPTIONAL { ?r slo:altitude ?altitude . } "
        "  OPTIONAL { ?r nmm:exposureTime ?exposureTime . } "
        "  OPTIONAL { ?r nmm:fNumber ?fNumber . } "
        "  OPTIONAL { ?r nmm:fnumber ?fNumber . } "
        "  OPTIONAL { ?r nmm:flash ?flashEnabled . } "
        "  OPTIONAL { ?r nmm:focalLength ?focalLength . } "
        "  OPTIONAL { ?r nmm:meteringMode ?meteringMode . } "
        "  OPTIONAL { ?r nmm:whiteBalance ?whiteBalance . } "
        "  OPTIONAL { ?r nfo:equipment ?eq . ?eq nfo:manufacturer ?cameraManufacturer . } "
        "  OPTIONAL { ?r nfo:equipment ?eq2 . ?eq2 nfo:model ?cameraModel . } "
        "  OPTIONAL { ?r nfo:duration ?duration . } "
        "  OPTIONAL { ?r nfo:frameRate ?frameRate . } "
        "  OPTIONAL { ?r nie:contentCreated ?created . } "
        "  OPTIONAL { ?r nie:contentCreated ?dateTaken . } "
        "  FILTER(BOUND(?url)) "
        "} "
        "ORDER BY ASC(%2)");

    return sparql.arg(typeFilter, orderExpr);
}

void TrackerGalleryModel::refresh()
{
    const quint64 generation = ++m_queryGeneration;
    setLoading(true);
    setError(false);

    const QString sparql = buildQuery();

    // Run the blocking Tracker query off the UI thread.
    std::thread([this, generation, sparql]() {
        bool ok = false;
        QVector<QVariantMap> results = runQuery(sparql, ok);
        QMetaObject::invokeMethod(this, [this, generation, ok, results]() {
            if (generation != m_queryGeneration)
                return; // stale result
            applyResults(results, ok); }, Qt::QueuedConnection);
    }).detach();
}
