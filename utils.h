#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqmlregistration.h>

class Utils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(Utils)

public:
    explicit Utils(QObject *parent = nullptr);
    ~Utils() override;

    Q_INVOKABLE void copyToClipboard(const QString &text);
};

