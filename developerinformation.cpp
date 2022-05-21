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
#include <QtCore/qsysinfo.h>

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

QString DeveloperInformation::qtRuntimeVersion() const
{
    return QString::fromUtf8(qVersion());
}

QString DeveloperInformation::qtCompiletimeVersion() const
{
    return QString::fromUtf8(QT_VERSION_STR);
}

QString DeveloperInformation::qtAbiInfo() const
{
    return QSysInfo::buildAbi();
}

QString DeveloperInformation::compilerInfo() const
{
    return QString::fromUtf8(COMPILER_STRING);
}

QString DeveloperInformation::systemInfo() const
{
    return (u"%1 (%2 %3)"_qs).arg(QSysInfo::prettyProductName(), QSysInfo::kernelType(), QSysInfo::kernelVersion());
}

QString DeveloperInformation::cpuArchitecture() const
{
    return QSysInfo::currentCpuArchitecture();
}
