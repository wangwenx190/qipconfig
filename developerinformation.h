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

#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqmlregistration.h>

class DeveloperInformation : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(DeveloperInformation)

    Q_PROPERTY(QString qtRunTimeVersion READ qtRunTimeVersion CONSTANT FINAL)
    Q_PROPERTY(QString qtCompileTimeVersion READ qtCompileTimeVersion CONSTANT FINAL)
    Q_PROPERTY(QString qtAbiInfo READ qtAbiInfo CONSTANT FINAL)
    Q_PROPERTY(QString compilerInfo READ compilerInfo CONSTANT FINAL)
    Q_PROPERTY(QString systemInfo READ systemInfo CONSTANT FINAL)
    Q_PROPERTY(QString cpuArchitecture READ cpuArchitecture CONSTANT FINAL)
    Q_PROPERTY(QString compileDateTime READ compileDateTime CONSTANT FINAL)

public:
    explicit DeveloperInformation(QObject *parent = nullptr);
    ~DeveloperInformation() override;

    [[nodiscard]] QString qtRunTimeVersion() const;
    [[nodiscard]] QString qtCompileTimeVersion() const;
    [[nodiscard]] QString qtAbiInfo() const;
    [[nodiscard]] QString compilerInfo() const;
    [[nodiscard]] QString systemInfo() const;
    [[nodiscard]] QString cpuArchitecture() const;
    [[nodiscard]] QString compileDateTime() const;
};
