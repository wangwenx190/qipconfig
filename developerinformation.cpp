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
