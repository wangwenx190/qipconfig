/*
 * MIT License
 *
 * Copyright (C) 2022 by wangwenx190 (Yuhang Zhao)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "utils.h"
#include <QtCore/qdatastream.h>
#include <QtCore/qvariant.h>
#include <QtCore/qsettings.h>
#include <QtGui/qguiapplication.h>
#include <QtGui/qclipboard.h>

static constexpr const QByteArray::Base64Options kEncryptOptions = (QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
static constexpr const QDataStream::Version kFormatVersion = QDataStream::Qt_6_3;
static constexpr const quint32 kUniqueIdentifier = 0xA0B0C0D0;

static const QString kGeometryKey = u"Window/Geometry"_qs;
static const QString kStateKey = u"Window/State"_qs;

[[nodiscard]] static inline QString applicationUri()
{
    static const QString uri = (u"org.%1.%2/%3"_qs).arg(
          QCoreApplication::organizationName(),
          QCoreApplication::applicationName(),
          QCoreApplication::applicationVersion());
    return uri;
}

[[nodiscard]] static inline QByteArray encrypt(const QString &str)
{
    Q_ASSERT(!str.isEmpty());
    if (str.isEmpty()) {
        return {};
    }
    const QByteArray ba = str.toUtf8();
    return ba.toBase64(kEncryptOptions);
}

[[nodiscard]] static inline QString decrypt(const QByteArray &ba)
{
    Q_ASSERT(!ba.isEmpty());
    if (ba.isEmpty()) {
        return {};
    }
    const QByteArray::FromBase64Result result = QByteArray::fromBase64Encoding(ba, kEncryptOptions);
    if (!result) {
        qWarning() << "Failed to decrypt Base64 data.";
        return {};
    }
    return QString::fromUtf8(*result);
}

[[nodiscard]] static inline QByteArray encode(const QVariant &var)
{
    Q_ASSERT(var.isValid());
    if (!var.isValid()) {
        return {};
    }
    QByteArray result = {};
    QDataStream stream(&result, QDataStream::WriteOnly);
    stream.setVersion(kFormatVersion);
    stream << kUniqueIdentifier << encrypt(applicationUri()) << var;
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
    QByteArray uri = {};
    stream >> uri;
    if (decrypt(uri) != applicationUri()) {
        qWarning() << "Settings: product URI does not match.";
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
