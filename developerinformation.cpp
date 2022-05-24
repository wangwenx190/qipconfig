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

#include "developerinformation.h"
#include <QtCore/qlibraryinfo.h>
#include <QtCore/qsysinfo.h>
#include <QtQuick/qquickwindow.h>

#ifndef COMPILER_STRING
#  ifdef Q_CC_CLANG // Must be before GNU, because Clang claims to be GNU too.
#    define COMPILER_STRING __VERSION__ // Already includes the compiler's name.
#  elif defined(Q_CC_GHS)
#    define COMPILER_STRING "GHS " QT_STRINGIFY(__GHS_VERSION_NUMBER)
#  elif defined(Q_CC_GNU)
#    define COMPILER_STRING "GCC " __VERSION__
#  elif defined(Q_CC_MSVC)
#    if (_MSC_VER < 1910)
#      define COMPILER_STRING "MSVC 2015"
#    elif (_MSC_VER < 1917)
#      define COMPILER_STRING "MSVC 2017"
#    elif (_MSC_VER < 1930)
#      define COMPILER_STRING "MSVC 2019"
#    elif (_MSC_VER < 2000)
#      define COMPILER_STRING "MSVC 2022"
#    else
#      define COMPILER_STRING "MSVC version " QT_STRINGIFY(_MSC_VER)
#    endif
#  else
#    define COMPILER_STRING "UNKNOWN"
#  endif
#endif

DeveloperInformation::DeveloperInformation(QObject *parent) : QObject(parent)
{
}

DeveloperInformation::~DeveloperInformation() = default;

QString DeveloperInformation::qtRunTimeVersion() const
{
    static const QString result = QString::fromUtf8(qVersion());
    return result;
}

QString DeveloperInformation::qtCompileTimeVersion() const
{
    static const QString result = QString::fromUtf8(QT_VERSION_STR);
    return result;
}

QString DeveloperInformation::qtAbiInfo() const
{
    static const QString result = QSysInfo::buildAbi();
    return result;
}

QString DeveloperInformation::compilerInfo() const
{
    static const QString result = QString::fromUtf8(COMPILER_STRING);
    return result;
}

QString DeveloperInformation::systemInfo() const
{
    static const QString result = (u"%1 (%2 %3)"_qs).arg(QSysInfo::prettyProductName(), QSysInfo::kernelType(), QSysInfo::kernelVersion());
    return result;
}

QString DeveloperInformation::cpuArchitecture() const
{
    static const QString result = QSysInfo::currentCpuArchitecture();
    return result;
}

QString DeveloperInformation::compileDateTime() const
{
    static const QString result = QString::fromUtf8(__DATE__ " " __TIME__);
    return result;
}

QString DeveloperInformation::graphicsApi() const
{
    static const QString result = []() -> QString {
        switch (QQuickWindow::graphicsApi()) {
        case QSGRendererInterface::Direct3D11:
            return u"Direct3D11"_qs;
        case QSGRendererInterface::Vulkan:
            return u"Vulkan"_qs;
        case QSGRendererInterface::Metal:
            return u"Metal"_qs;
        case QSGRendererInterface::OpenGL:
            return u"OpenGL"_qs;
        case QSGRendererInterface::Software:
            return u"Software"_qs;
        default:
            break;
        }
        return tr("UNKNOWN");
    }();
    return result;
}

QString DeveloperInformation::qtBuildInfo() const
{
    static const QString result = QString::fromUtf8(QLibraryInfo::build());
    return result;
}

QString DeveloperInformation::appCommitHash() const
{
    static const QString result = QString::fromUtf8(PROJECT_VERSION_COMMIT);
    return result;
}

QString DeveloperInformation::appCommitHashShort() const
{
    static const QString result = appCommitHash().first(6);
    return result;
}
