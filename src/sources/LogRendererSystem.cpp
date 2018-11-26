#include <LogRendererSystem.hpp>

shared_ptr<UIElement> LogRendererSystem::logDisplayer = nullptr;
vec4 LogRendererSystem::logNormalColor = vec4(0.26f, 0.91f, 0.96f, 0.5f);
vec4 LogRendererSystem::logWarningColor = vec4(0.95f, 0.93f, 0.16f, 0.5f);
vec4 LogRendererSystem::logErrorColor = vec4(0.8f, 0.f, 0.f, 0.5f);
vector<LogMessage> LogRendererSystem::logMessages;
bool LogRendererSystem::initialized = false;

void LogRendererSystem::startUp(void) {
    if (!initialized) {
        LogRendererSystem::logDisplayer = make_shared<UIElement>();
        LogRendererSystem::logDisplayer->getState().setWidth(400.f);
        LogRendererSystem::logDisplayer->getState().setHeight(400.f);
        LogRendererSystem::logDisplayer->getState().setWordWrap(true);
        LogRendererSystem::logDisplayer->getState().setZIndex(1e6f);
        LogRendererSystem::logDisplayer->getState().setFillColor(vec4(0.f, 0.f, 0.f, 0.4f));
        LogRendererSystem::logDisplayer->getState().setFontKey(FontKey::ARIAL);
        LogRendererSystem::logDisplayer->getState().setFontSize(14.f);
        UIUtils::translate(LogRendererSystem::logDisplayer, 200.f, 200.f);
        LogRendererSystem::logDisplayer->getState().setTextVerticalAlign(TextVerticalAlign::TOP);
        initialized = true;
    }
}

void LogRendererSystem::shutDown(void) {
    if (initialized) {
        LogRendererSystem::logDisplayer = nullptr;
        initialized = false;
    }
}

bool LogRendererSystem::isInitialized(void) {
    return LogRendererSystem::initialized;
}

void LogRendererSystem::pushLogMessage(const string& message, const GLfloat& timeAllowedForDisplay, const LogMessageType& type) {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }

    switch (type) {
        case LogMessageType::LOG_NORMAL: {
            logMessages.push_back(LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logNormalColor));
            break;
        }
        case LogMessageType::LOG_WARNING: {
            logMessages.push_back(LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logWarningColor));
            break;
        }
        case LogMessageType::LOG_ERROR: {
            logMessages.push_back(LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logErrorColor));
            break;
        }
    }
}

void LogRendererSystem::overwriteTopLogMessage(const string& message, const GLfloat& timeAllowedForDisplay, const LogMessageType& type) {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }
    if (LogRendererSystem::logMessages.empty()) {
        switch (type) {
            case LogMessageType::LOG_NORMAL: {
                logMessages.push_back(LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logNormalColor));
                break;
            }
            case LogMessageType::LOG_WARNING: {
                logMessages.push_back(LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logWarningColor));
                break;
            }
            case LogMessageType::LOG_ERROR: {
                logMessages.push_back(LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logErrorColor));
                break;
            }
        }
    }
    else {
        switch (type) {
            case LogMessageType::LOG_NORMAL: {
                logMessages[logMessages.size() - 1] = LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logNormalColor);
                break;
            }
            case LogMessageType::LOG_WARNING: {
                logMessages[logMessages.size() - 1] = LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logWarningColor);
                break;
            }
            case LogMessageType::LOG_ERROR: {
                logMessages[logMessages.size() - 1] = LogMessage(message, timeAllowedForDisplay, type, LogRendererSystem::logErrorColor);
                break;
            }
        }
    }
}

void LogRendererSystem::setUIShader(const shared_ptr<Shader>& uiShader) noexcept {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }

    LogRendererSystem::logDisplayer->setShader(uiShader);
}

void LogRendererSystem::setTextShader(const shared_ptr<Shader>& textShader) noexcept {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }
    
    LogRendererSystem::logDisplayer->setTextShader(textShader);
}

void LogRendererSystem::setLogNormalColor(const vec4& logNormalColor) noexcept {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }
    
    LogRendererSystem::logNormalColor = logNormalColor;

    for (auto it = LogRendererSystem::logMessages.begin(); it != LogRendererSystem::logMessages.end(); ++it) {
        if ((*it).getType() == LogMessageType::LOG_NORMAL) {
            (*it).setTextColor(LogRendererSystem::logNormalColor);
        }
    }
}

void LogRendererSystem::setLogWarningColor(const vec4& logWarningColor) noexcept {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }
    
    LogRendererSystem::logWarningColor = logWarningColor;

    for (auto it = LogRendererSystem::logMessages.begin(); it != LogRendererSystem::logMessages.end(); ++it) {
        if ((*it).getType() == LogMessageType::LOG_WARNING) {
            (*it).setTextColor(LogRendererSystem::logWarningColor);
        }
    }
}

void LogRendererSystem::setLogErrorColor(const vec4& logErrorColor) noexcept {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }
    
    LogRendererSystem::logErrorColor = logErrorColor;

    for (auto it = LogRendererSystem::logMessages.begin(); it != LogRendererSystem::logMessages.end(); ++it) {
        if ((*it).getType() == LogMessageType::LOG_ERROR) {
            (*it).setTextColor(LogRendererSystem::logErrorColor);
        }
    }
}

void LogRendererSystem::renderMessages(const mat4& ProjectionViewMatrix) {
    if (!isInitialized()) {
        cout << "Log Renderer System has not been initialized!" << endl;
        return;
    }

    LogRendererSystem::logDisplayer->draw(ProjectionViewMatrix);
    for (auto it = LogRendererSystem::logMessages.begin(); it != LogRendererSystem::logMessages.end(); it++) {
        LogMessage& logMessage = *it;

        if (!logMessage.shouldMessageBeDisplayed()) {
            it = LogRendererSystem::logMessages.erase(it);
            if (it == LogRendererSystem::logMessages.end()) {
                break;
            }
        }
    }

    TextRenderingSystem::renderText(
        LogRendererSystem::logDisplayer->getTextShader(),
        LogRendererSystem::logDisplayer->getState(),
        LogRendererSystem::logMessages,
        ProjectionViewMatrix,
        LogRendererSystem::logDisplayer->getTransform().getMatrix()
    );
}