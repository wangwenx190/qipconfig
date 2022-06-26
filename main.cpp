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

#include <QtCore/qlocale.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qcommandlineparser.h>
#include <QtGui/qfontdatabase.h>
#include <QtNetwork/qnetworkproxy.h>
#include <QtQml/qqmlapplicationengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQuick/qsgrendererinterface.h>
#include <QtQuick/qquickwindow.h>
#include <QtQuickControls2/qquickstyle.h>
#include <framelessquickmodule.h>
#include <private/framelessconfig_p.h>
#include <qtacrylicmaterialplugin.h>
#include <singleapplication.h>
#include "theme.h"
#include "translationmanager.h"
#include "networkadaptermodel.h"
#ifdef Q_OS_WINDOWS
#  include <QtCore/qt_windows.h>
#endif

#if (QT_VERSION < QT_VERSION_CHECK(6, 4, 0))
#  error Please upgrade your Qt SDK to the latest version!
#endif

FRAMELESSHELPER_USE_NAMESPACE

[[nodiscard]] static inline bool isInsensitiveEqual(const QString &lhs, const QString &rhs)
{
    Q_ASSERT(!lhs.isEmpty());
    Q_ASSERT(!rhs.isEmpty());
    if (lhs.isEmpty() || rhs.isEmpty()) {
        return false;
    }
    return (QString::compare(lhs, rhs, Qt::CaseInsensitive) == 0);
}

[[nodiscard]] static inline TranslationManager::Language getPreferredLanguage(const QString &input)
{
    static const TranslationManager::Language defaultLanguage = []() -> TranslationManager::Language {
        const QString systemUiLanguage = QLocale::system().uiLanguages().constFirst();
        if ((systemUiLanguage == u"zh-Hans-CN"_qs)
            || (systemUiLanguage == u"zh-CN"_qs) || (systemUiLanguage == u"zh"_qs)) {
            return TranslationManager::Language::zh_Hans;
        }
        return TranslationManager::Language::en_US;
    }();
    if (input.isEmpty()) {
        return defaultLanguage;
    }
    if (isInsensitiveEqual(input, u"zh-hans-cn"_qs) || isInsensitiveEqual(input, u"zh-hans"_qs)
        || isInsensitiveEqual(input, u"zh-cn"_qs) || isInsensitiveEqual(input, u"zh"_qs)
        || isInsensitiveEqual(input, u"cn"_qs)) {
        return TranslationManager::Language::zh_Hans;
    }
    return defaultLanguage;
}

[[nodiscard]] static inline Theme::ThemeType getPreferredTheme(const QString &input)
{
    if (input.isEmpty()) {
        return Theme::ThemeType::System;
    }
    if (isInsensitiveEqual(input, u"light"_qs)) {
        return Theme::ThemeType::Light;
    }
    if (isInsensitiveEqual(input, u"dark"_qs)) {
        return Theme::ThemeType::Dark;
    }
    return Theme::ThemeType::System;
}

[[nodiscard]] static inline std::optional<QSGRendererInterface::GraphicsApi> getPreferredGraphicsApi(const QString &input)
{
    if (input.isEmpty()) {
        return std::nullopt;
    }
#ifdef Q_OS_WINDOWS
    if (isInsensitiveEqual(input, u"d3d11"_qs) || isInsensitiveEqual(input, u"direct3d11"_qs)
        || isInsensitiveEqual(input, u"d3d"_qs) || isInsensitiveEqual(input, u"direct3d"_qs)
        || isInsensitiveEqual(input, u"dx"_qs) || isInsensitiveEqual(input, u"directx"_qs)
        || isInsensitiveEqual(input, u"win32"_qs) || isInsensitiveEqual(input, u"windows"_qs)
        || isInsensitiveEqual(input, u"microsoft"_qs)) {
        return QSGRendererInterface::Direct3D11;
    }
#endif
    if (isInsensitiveEqual(input, u"vulkan"_qs) || isInsensitiveEqual(input, u"vk"_qs)
        || isInsensitiveEqual(input, u"linux"_qs) || isInsensitiveEqual(input, u"unix"_qs)) {
        return QSGRendererInterface::Vulkan;
    }
#ifdef Q_OS_MACOS
    if (isInsensitiveEqual(input, u"metal"_qs) || isInsensitiveEqual(input, u"mt"_qs)
        || isInsensitiveEqual(input, u"macos"_qs) || isInsensitiveEqual(input, u"mac"_qs)
        || isInsensitiveEqual(input, u"darwin"_qs) || isInsensitiveEqual(input, u"apple"_qs)) {
        return QSGRendererInterface::Metal;
    }
#endif
    if (isInsensitiveEqual(input, u"opengl"_qs) || isInsensitiveEqual(input, u"gl"_qs)
        || isInsensitiveEqual(input, u"opengles"_qs) || isInsensitiveEqual(input, u"gles"_qs)
        || isInsensitiveEqual(input, u"legacy"_qs)) {
        return QSGRendererInterface::OpenGL;
    }
    if (isInsensitiveEqual(input, u"software"_qs) || isInsensitiveEqual(input, u"sw"_qs)
        || isInsensitiveEqual(input, u"cpu"_qs) || isInsensitiveEqual(input, u"compat"_qs)) {
        return QSGRendererInterface::Software;
    }
    return std::nullopt;
}

int main(int argc, char *argv[])
{
    FramelessHelper::Core::initialize();

    QCoreApplication::setApplicationName(u"QIPConfig"_qs);
    QGuiApplication::setApplicationDisplayName(u"QIPConfig"_qs);
    QCoreApplication::setApplicationVersion(u"1.0.0.0"_qs);
    QCoreApplication::setOrganizationName(u"wangwenx190"_qs);
    QCoreApplication::setOrganizationDomain(u"https://github.com/wangwenx190/QIPConfig/"_qs);

    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);

    SingleApplication application(argc, argv, true, (SingleApplication::User | SingleApplication::SecondaryNotification));

    if (application.isSecondary()) {
        qDebug() << "Detected primary instance exists, exiting now ...";
#ifdef Q_OS_WINDOWS
        if (AllowSetForegroundWindow(static_cast<DWORD>(application.primaryPid())) == FALSE) {
            qWarning() << "AllowSetForegroundWindow() failed with error code" << GetLastError();
        }
#endif
        QStringList arguments = application.arguments();
        arguments.takeFirst();
        if (!arguments.isEmpty()) {
            const QString params = arguments.join(u' ');
            if (application.sendMessage(params.toUtf8())) {
                qDebug() << "Message" << params << "has been sent to the primary instance.";
            } else {
                qWarning() << "Failed to send message" << params << "to the primary instance.";
                return -1;
            }
        }
        return 0;
    }
    QObject::connect(&application, &SingleApplication::receivedMessage, QCoreApplication::instance(), [](const quint32 instanceId, const QByteArray &message){
        qDebug() << "Message received from secondary instance" << instanceId << ':' << QString::fromUtf8(message);
    });

    QCommandLineParser commandLine;
    commandLine.setApplicationDescription(QCoreApplication::translate("main", "A convenient tool to show your network configuration."));
    commandLine.addHelpOption();
    commandLine.addVersionOption();

    const QCommandLineOption themeOption(u"theme"_qs,
        QCoreApplication::translate("main", "Set the application theme to <theme>. Available values: light, dark, system."),
        QCoreApplication::translate("main", "theme"));
    commandLine.addOption(themeOption);

    const QCommandLineOption languageOption(u"language"_qs,
        QCoreApplication::translate("main", "Set the application language to <language>. Available values: zh-hans, en-us, auto."),
        QCoreApplication::translate("main", "language"));
    commandLine.addOption(languageOption);

    const QCommandLineOption graphicsApiOption(u"graphics-api"_qs,
        QCoreApplication::translate("main", "Set the renderer's 3D graphics API to <API>. Available values: d3d11, vulkan, metal, opengl, software, auto."),
        QCoreApplication::translate("main", "API"));
    commandLine.addOption(graphicsApiOption);

    const QCommandLineOption useSystemProxyOption(u"use-system-proxy"_qs,
        QCoreApplication::translate("main", "Use the system network proxy configuration."));
    commandLine.addOption(useSystemProxyOption);

    commandLine.process(application);

    QLoggingCategory::setFilterRules(u"qt.scenegraph.general=true\nqt.rhi.*=true"_qs);

    QNetworkProxyFactory::setUseSystemConfiguration(commandLine.isSet(useSystemProxyOption));

    FramelessConfig::setLoadFromEnvironmentVariablesDisabled();
    FramelessConfig::setLoadFromConfigurationFileDisabled();
    FramelessConfig::instance()->set(Global::Option::WindowUseRoundCorners);

    Theme::setPreferredTheme(getPreferredTheme(commandLine.value(themeOption)));

    QFontDatabase::addApplicationFont(u":/fonts/Segoe Fluent Icons.ttf"_qs);

    const std::optional<QSGRendererInterface::GraphicsApi> preferredGraphicsApi = getPreferredGraphicsApi(commandLine.value(graphicsApiOption));
    if (preferredGraphicsApi.has_value()) {
        QQuickWindow::setGraphicsApi(preferredGraphicsApi.value());
    }

    QQuickStyle::setStyle(u"Basic"_qs);

    NetworkAdapterModel model;

    QQmlApplicationEngine engine;

    TranslationManager::instance()->setQmlEngine(&engine);

    FramelessHelper::Quick::registerTypes(&engine);
    QtAcrylicMaterial::registerTypes(&engine);

    engine.rootContext()->setContextProperty(u"networkAdapterModel"_qs, &model);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, QCoreApplication::instance(),
        [](const QUrl &url){
            qCritical() << "Failed to create object" << url;
            QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(QUrl(u"qrc:///QIPConfig/qml/main.qml"_qs));

    const QObjectList rootObjects = engine.rootObjects();
    Q_ASSERT(!rootObjects.isEmpty());
    if (rootObjects.isEmpty()) {
        qWarning() << "The QML engine does not create any elements.";
        return -1;
    }
    QObject::connect(&application, &SingleApplication::instanceStarted, QCoreApplication::instance(), [&rootObjects](){
        qDebug() << "Woken up by secondary instance.";
        QMetaObject::invokeMethod(rootObjects.constFirst(), "raiseMainWindow");
    });

    TranslationManager::instance()->setLanguage(getPreferredLanguage(commandLine.value(languageOption)));

    return QCoreApplication::exec();
}
