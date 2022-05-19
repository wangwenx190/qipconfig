#pragma once

#include <QtCore/qobject.h>
#include <QtGui/qwindow.h>
#include <QtQml/qqmlregistration.h>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class Utils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(Utils)

public:
    explicit Utils(QObject *parent = nullptr);
    ~Utils() override;

public Q_SLOTS:
    void copyToClipboard(const QString &text);
    void saveGeometry(QWindow *window);
    [[nodiscard]] bool restoreGeometry(QWindow *window);

private:
    QScopedPointer<QSettings> m_settings;
};

