#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqmlregistration.h>

class DeveloperInformation : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(DeveloperInformation)

    Q_PROPERTY(QString qtRuntimeVersion READ qtRuntimeVersion CONSTANT FINAL)
    Q_PROPERTY(QString qtCompiletimeVersion READ qtCompiletimeVersion CONSTANT FINAL)
    Q_PROPERTY(QString qtAbiInfo READ qtAbiInfo CONSTANT FINAL)
    Q_PROPERTY(QString compilerInfo READ compilerInfo CONSTANT FINAL)
    Q_PROPERTY(QString systemInfo READ systemInfo CONSTANT FINAL)
    Q_PROPERTY(QString cpuArchitecture READ cpuArchitecture CONSTANT FINAL)

public:
    explicit DeveloperInformation(QObject *parent = nullptr);
    ~DeveloperInformation() override;

    [[nodiscard]] QString qtRuntimeVersion() const;
    [[nodiscard]] QString qtCompiletimeVersion() const;
    [[nodiscard]] QString qtAbiInfo() const;
    [[nodiscard]] QString compilerInfo() const;
    [[nodiscard]] QString systemInfo() const;
    [[nodiscard]] QString cpuArchitecture() const;
};

