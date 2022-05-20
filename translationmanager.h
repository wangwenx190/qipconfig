#pragma once

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

QT_BEGIN_NAMESPACE
class QQmlEngine;
class QTranslator;
QT_END_NAMESPACE

class TranslationManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(TranslationManager)
    Q_PROPERTY(Language language READ getLanguage WRITE setLanguage NOTIFY languageChanged FINAL)

public:
    enum class Language { en_US, zh_Hans, Default = en_US };
    Q_ENUM(Language)

    explicit TranslationManager(QObject *parent = nullptr);
    ~TranslationManager() override;

    [[nodiscard]] static TranslationManager *instance();

    void setQmlEngine(QQmlEngine *engine);

    [[nodiscard]] Language getLanguage() const;
    void setLanguage(const Language lang);

Q_SIGNALS:
    void languageChanged();

private:
    Language m_language = Language::en_US;
    QPointer<QQmlEngine> m_engine = nullptr;
    QScopedPointer<QTranslator> m_translator_zh_Hans;
};

