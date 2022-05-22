#[[
  MIT License

  Copyright (C) 2022 by wangwenx190 (Yuhang Zhao)

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
]]

cmake_minimum_required(VERSION 3.23)

project(QIPConfig
    VERSION 1.0.0.0
    DESCRIPTION "A convenient tool to show your network configuration."
    HOMEPAGE_URL "https://github.com/wangwenx190/qipconfig/"
    LANGUAGES CXX
)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    if(NOT DEFINED CMAKE_INTERPROCEDURAL_OPTIMIZATION)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX d)
    else()
        set(CMAKE_DEBUG_POSTFIX _debug)
    endif()
endif()

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_SHARED_LIBS OFF)

# FramelessHelper
set(FRAMELESSHELPER_BUILD_STATIC ON)
set(FRAMELESSHELPER_BUILD_WIDGETS OFF)
set(FRAMELESSHELPER_BUILD_EXAMPLES OFF)

# QtAcrylicMaterial
set(QTACRYLICMATERIAL_BUILD_DEMO OFF)
set(QTACRYLICMATERIAL_BUILD_STATIC ON)

# SingleApplication
set(QAPPLICATION_CLASS QGuiApplication)

set(translations_dir ${CMAKE_CURRENT_SOURCE_DIR}/translations)

find_package(Qt6 REQUIRED COMPONENTS
    Core Network Gui ShaderTools Quick
    QuickControls2 LinguistTools
)

qt_add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    resources.qrc
    networkadapter.h
    networkadaptermodel.h networkadaptermodel.cpp
    networkinformation.h networkinformation.cpp
    utils.h utils.cpp
    theme.h theme.cpp
    themehelper.h themehelper.cpp
    translationmanager.h translationmanager.cpp
    developerinformation.h developerinformation.cpp
    main.cpp
)

if(WIN32)
    enable_language(RC)
    target_sources(${PROJECT_NAME} PRIVATE
        app.rc app.manifest theme_win.cpp
    )
elseif(APPLE)
    target_sources(${PROJECT_NAME} PRIVATE theme_mac.mm)
elseif(UNIX)
    target_sources(${PROJECT_NAME} PRIVATE theme_linux.cpp)
endif()

set(QML_SOURCES
    qml/RefreshButton.qml
    qml/ClickableLabel.qml
    qml/MainWindow.qml
)

qt_add_qml_module(${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    IMPORTS QtQuick
    DEPENDENCIES
        org.wangwenx190.FramelessHelper
        org.wangwenx190.QtAcrylicMaterial
    QML_FILES ${QML_SOURCES}
)

qt_add_translations(${PROJECT_NAME}
    TS_FILES
        ${translations_dir}/${PROJECT_NAME}_zh_Hans.ts
    LUPDATE_OPTIONS
        -no-obsolete -no-recursive
    LRELEASE_OPTIONS
        -compress -nounfinished -removeidentical
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER org.wangwenx190.${PROJECT_NAME}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_TO_ASCII
    QT_NO_URL_CAST_FROM_STRING
    QT_NO_CAST_FROM_BYTEARRAY
    #QT_NO_KEYWORDS
    QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    QT_NO_FOREACH
    QT_USE_QSTRINGBUILDER
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060400
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::CorePrivate Qt::Network Qt::GuiPrivate
    Qt::Quick Qt::QuickControls2
    FramelessHelperCore FramelessHelperQuick
    QtAcrylicMaterial
    SingleApplication::SingleApplication
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /utf-8 /W3 /WX
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Werror
    )
endif()

add_subdirectory(3rdparty)
