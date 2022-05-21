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

#include "translationmanager.h"
#include <QtCore/qtranslator.h>
#include <QtCore/qcoreapplication.h>
#include <QtQml/qqmlengine.h>

static struct {
    QTranslator *translator = nullptr;
    QString fileName = {};
} g_availableTranslators[] = {
    { nullptr, u"QIPConfig_en_US"_qs }, // Just a placeholder.
    { nullptr, u"QIPConfig_zh_Hans"_qs }
};

static constexpr const auto g_translatorCount = int(std::size(g_availableTranslators));
static constexpr const int g_translatorOffset = (static_cast<int>(TranslationManager::Language::en_US) + 1);

Q_GLOBAL_STATIC(TranslationManager, translationManager)

TranslationManager::TranslationManager(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Language>();
    for (int i = g_translatorOffset; i != g_translatorCount; ++i) {
        auto translator = new QTranslator;
        if (translator->load(g_availableTranslators[i].fileName, u":/i18n"_qs)) {
            g_availableTranslators[i].translator = translator;
        } else {
            delete translator;
            translator = nullptr;
            qWarning() << "Failed to load translation from" << g_availableTranslators[i].fileName;
        }
    }
}

TranslationManager::~TranslationManager()
{
    for (int i = g_translatorOffset; i != g_translatorCount; ++i) {
        if (g_availableTranslators[i].translator) {
            delete g_availableTranslators[i].translator;
            g_availableTranslators[i].translator = nullptr;
        }
    }
}

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
    uninstallAllTranslators();
    if (QTranslator * const translator = g_availableTranslators[static_cast<int>(lang)].translator) {
        qApp->installTranslator(translator);
    }
    if (m_engine) {
        m_engine->retranslate();
    }
    Q_EMIT languageChanged();
}

void TranslationManager::uninstallAllTranslators()
{
    for (int i = g_translatorOffset; i != g_translatorCount; ++i) {
        if (g_availableTranslators[i].translator) {
            qApp->removeTranslator(g_availableTranslators[i].translator);
        }
    }
}
