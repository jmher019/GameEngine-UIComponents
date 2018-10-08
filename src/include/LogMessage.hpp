#ifndef LOG_MESSAGE_HPP
#define LOG_MESSAGE_HPP

#include <Vertex.hpp>
#include <Timer.hpp>

using namespace std;

enum class LogMessageType {
    LOG_NORMAL,
    LOG_WARNING,
    LOG_ERROR
};

class LogMessage {
private:
    string message;
    GLfloat timeAllowedForDisplay;
    vec4 textColor;
    Timer timer;
    LogMessageType type;

public:
    LogMessage(const string& message, const GLfloat& timeAllowedForDisplay, const LogMessageType& type, const vec4& textColor);

    LogMessage(const LogMessage& logMessage);

    LogMessage(LogMessage&& logMessage);

    LogMessage& operator=(const LogMessage& logMessage) noexcept;

    LogMessage& operator=(LogMessage&& logMessage) noexcept;

    const string& getMessage(void) const noexcept;

    void setTextColor(const vec4& textColor) noexcept;

    const vec4& getTextColor(void) const noexcept;

    const LogMessageType& getType(void) const noexcept;

    bool shouldMessageBeDisplayed(void) noexcept;
};

#endif // !LOG_MESSAGE_HPP
