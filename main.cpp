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

#include <QtCore/qloggingcategory.h>
#include <QtCore/qcommandlineparser.h>
#include <QtNetwork/qnetworkproxy.h>
#include <QtGui/qguiapplication.h>
#include <QtQml/qqmlapplicationengine.h>
#include <QtQml/qqmlcontext.h>
#include <QtQuick/qsgrendererinterface.h>
#include <QtQuick/qquickwindow.h>
#include <QtQuickControls2/qquickstyle.h>
#include <framelessquickmodule.h>
#include <qtacrylicmaterialplugin.h>
#include "themehelper.h"
#include "networkadaptermodel.h"

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

[[nodiscard]] static inline ThemeHelper::Theme getPreferredTheme(const QString &input)
{
    if (input.isEmpty()) {
        return ThemeHelper::Theme::System;
    }
    if (isInsensitiveEqual(input, u"light"_qs)) {
        return ThemeHelper::Theme::Light;
    }
    if (isInsensitiveEqual(input, u"dark"_qs)) {
        return ThemeHelper::Theme::Dark;
    }
    return ThemeHelper::Theme::System;
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
    QCoreApplication::setOrganizationDomain(u"wangwenx190.github.io"_qs);

    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);

    QGuiApplication application(argc, argv);

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
        QCoreApplication::translate("main", "Set the rendering 3D graphics API to <API>. Available values: d3d11, vulkan, metal, opengl, software, auto."),
        QCoreApplication::translate("main", "API"));
    commandLine.addOption(graphicsApiOption);

    const QCommandLineOption useSystemProxyOption(u"use-system-proxy"_qs,
        QCoreApplication::translate("main", "Use the system proxy configuration."));
    commandLine.addOption(useSystemProxyOption);

    commandLine.process(application);

    ThemeHelper::instance()->setPreferredTheme(getPreferredTheme(commandLine.value(themeOption)));

    const std::optional<QSGRendererInterface::GraphicsApi> preferredGraphicsApi = getPreferredGraphicsApi(commandLine.value(graphicsApiOption));
    if (preferredGraphicsApi.has_value()) {
        QQuickWindow::setGraphicsApi(preferredGraphicsApi.value());
    }

    QQuickStyle::setStyle(u"Basic"_qs);

    QNetworkProxyFactory::setUseSystemConfiguration(commandLine.isSet(useSystemProxyOption));

    QLoggingCategory::setFilterRules(u"qt.scenegraph.general=true\nqt.rhi.*=true"_qs);

    NetworkAdapterModel model;

    QQmlApplicationEngine engine;

    FramelessHelper::Quick::registerTypes(&engine);
    QtAcrylicMaterial::registerTypes(&engine);

    engine.rootContext()->setContextProperty(u"networkAdapterModel"_qs, &model);

    const QUrl mainUrl(u"qrc:///QIPConfig/MainWindow.qml"_qs);

    const QMetaObject::Connection connection = QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &application,
        [&mainUrl, &connection](QObject *object, const QUrl &objectUrl) {
            if (objectUrl != mainUrl) {
                return;
            }
            if (object) {
                QObject::disconnect(connection);
            } else {
                QCoreApplication::exit(-1);
            }
        }, Qt::QueuedConnection);

    engine.load(mainUrl);

    return QCoreApplication::exec();
}
