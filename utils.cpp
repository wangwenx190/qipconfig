#include "utils.h"
#include <QtCore/qdatastream.h>
#include <QtCore/qvariant.h>
#include <QtCore/qsettings.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qclipboard.h>

static constexpr const QDataStream::Version kFormatVersion = QDataStream::Qt_6_3;

static constexpr const quint32 kUniqueIdentifier = 0xA1B2C3D4;

static const QString kGeometryKey = u"Window/Geometry"_qs;
static const QString kStateKey = u"Window/State"_qs;

[[nodiscard]] static inline QByteArray encode(const QVariant &var)
{
    Q_ASSERT(var.isValid());
    if (!var.isValid()) {
        return {};
    }
    QByteArray result = {};
    QDataStream stream(&result, QDataStream::WriteOnly);
    stream.setVersion(kFormatVersion);
    stream << kUniqueIdentifier << QCoreApplication::applicationName()
           << QCoreApplication::applicationVersion() << var;
    return result;
}

[[nodiscard]] static inline QVariant decode(const QByteArray &ba)
{
    Q_ASSERT(!ba.isEmpty());
    if (ba.isEmpty()) {
        return {};
    }
    QDataStream stream(ba);
    stream.setVersion(kFormatVersion);
    quint32 identifier = 0;
    stream >> identifier;
    if (identifier != kUniqueIdentifier) {
        qWarning() << "Settings: data format not valid.";
        return {};
    }
    QString appName = {};
    stream >> appName;
    if (appName.compare(QCoreApplication::applicationName()) != 0) {
        qWarning() << "Settings: product does not match.";
        return {};
    }
    QString appVer = {};
    stream >> appVer;
    if (appVer.compare(QCoreApplication::applicationVersion()) != 0) {
        qWarning() << "Settings: version does not match.";
        return {};
    }
    QVariant var = {};
    stream >> var;
    return var;
}

Utils::Utils(QObject *parent) : QObject(parent)
{
    m_settings.reset(new QSettings(this));
}

Utils::~Utils() = default;

void Utils::copyToClipboard(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }
    QGuiApplication::clipboard()->setText(text);
}

void Utils::saveGeometry(QWindow *window)
{
    Q_ASSERT(window);
    if (!window) {
        return;
    }
    m_settings->setValue(kGeometryKey, encode(window->geometry()));
    m_settings->setValue(kStateKey, encode(window->windowState()));
}

bool Utils::restoreGeometry(QWindow *window)
{
    Q_ASSERT(window);
    if (!window) {
        return false;
    }
    const QByteArray geometryData = m_settings->value(kGeometryKey).toByteArray();
    if (geometryData.isEmpty()) {
        return false;
    }
    const QRect geometry = decode(geometryData).toRect();
    if (!geometry.isValid()) {
        return false;
    }
    const QByteArray stateData = m_settings->value(kStateKey).toByteArray();
    if (stateData.isEmpty()) {
        return false;
    }
    const auto state = qvariant_cast<Qt::WindowState>(decode(stateData));
    window->setGeometry(geometry);
    window->setWindowState(state);
    return true;
}
