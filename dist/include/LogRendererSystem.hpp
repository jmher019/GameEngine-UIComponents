#ifndef LOG_RENDERER_SYSTEM_HPP
#define LOG_RENDERER_SYSTEM_HPP

#include <Timer.hpp>
#include <UIElement.hpp>
#include <LogMessage.hpp>

class LogRendererSystem {
private:
    static unique_ptr<UIElement> logDisplayer;
    static vec4 logNormalColor;
    static vec4 logWarningColor;
    static vec4 logErrorColor;
    static vector<LogMessage> logMessages;
    static bool initialized;

public:
    static void startUp(void);

    static void shutDown(void);

    static bool isInitialized(void);

    static void pushLogMessage(const string& message, const GLfloat& timeAllowedForDisplay, const LogMessageType& type);

    static void overwriteTopLogMessage(const string& message, const GLfloat& timeAllowedForDisplay, const LogMessageType& type);

    static void setUIShader(const shared_ptr<Shader>& uiShader) noexcept;

    static void setTextShader(const shared_ptr<Shader>& textShader) noexcept;

    static void setLogNormalColor(const vec4& logNormalColor) noexcept;

    static void setLogWarningColor(const vec4& logNormalColor) noexcept;

    static void setLogErrorColor(const vec4& logNormalColor) noexcept;

    static void renderMessages(const mat4& ProjectionViewMatrix);
};

#endif // !LOG_RENDERER_SYSTEM_HPP
