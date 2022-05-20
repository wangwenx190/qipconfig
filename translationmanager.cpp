#include "translationmanager.h"
#include <QtCore/qtranslator.h>
#include <QtCore/qcoreapplication.h>
#include <QtQml/qqmlengine.h>

Q_GLOBAL_STATIC(TranslationManager, translationManager)

TranslationManager::TranslationManager(QObject *parent) : QObject(parent)
{
    m_translator_zh_Hans.reset(new QTranslator);
    if (!m_translator_zh_Hans->load(u"QIPConfig_zh_Hans"_qs, u":/i18n"_qs)) {
        qWarning() << "Failed to load the zh-Hans translation.";
    }
}

TranslationManager::~TranslationManager() = default;

TranslationManager *TranslationManager::instance()
{
    return translationManager();
}

void TranslationManager::setQmlEngine(QQmlEngine *engine)
{
    Q_ASSERT(engine);
    if (!engine) {
        return;
    }
    if (m_engine == engine) {
        return;
    }
    m_engine = engine;
}

TranslationManager::Language TranslationManager::getLanguage() const
{
    return m_language;
}

void TranslationManager::setLanguage(const Language lang)
{
    if (m_language == lang) {
        return;
    }
    m_language = lang;
    switch (m_language) {
    case Language::en_US: {
        qApp->removeTranslator(m_translator_zh_Hans.get());
    } break;
    case Language::zh_Hans: {
        qApp->installTranslator(m_translator_zh_Hans.get());
    } break;
    }
    if (m_engine) {
        m_engine->retranslate();
    }
    Q_EMIT languageChanged();
}
