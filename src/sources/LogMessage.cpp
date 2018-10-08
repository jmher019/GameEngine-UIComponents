#include <LogMessage.hpp>

LogMessage::LogMessage(const string& message, const GLfloat& timeAllowedForDisplay, const LogMessageType& type, const vec4& textColor) :
    message(message),
    timeAllowedForDisplay(timeAllowedForDisplay),
    textColor(textColor),
    timer(Timer()),
    type(type) {
}

LogMessage::LogMessage(const LogMessage& logMessage) :
    message(logMessage.message),
    timeAllowedForDisplay(logMessage.timeAllowedForDisplay),
    textColor(logMessage.textColor),
    timer(logMessage.timer),
    type(logMessage.type) {
}

LogMessage::LogMessage(LogMessage&& logMessage) :
    message(move(logMessage.message)),
    timeAllowedForDisplay(move(logMessage.timeAllowedForDisplay)),
    textColor(move(logMessage.textColor)),
    timer(move(logMessage.timer)),
    type(move(logMessage.type)) {
}

LogMessage& LogMessage::operator=(const LogMessage& logMessage) noexcept {
    message = logMessage.message;
    timeAllowedForDisplay = logMessage.timeAllowedForDisplay;
    textColor = logMessage.textColor;
    timer = logMessage.timer;
    type = logMessage.type;

    return *this;
}

LogMessage& LogMessage::operator=(LogMessage&& logMessage) noexcept {
    message = move(logMessage.message);
    timeAllowedForDisplay = move(logMessage.timeAllowedForDisplay);
    textColor = move(logMessage.textColor);
    timer = move(logMessage.timer);
    type = move(logMessage.type);

    return *this;
}

const string& LogMessage::getMessage(void) const noexcept {
    return message;
}

void LogMessage::setTextColor(const vec4& textColor) noexcept {
    this->textColor = textColor;
}

const vec4& LogMessage::getTextColor(void) const noexcept {
    return textColor;
}

const LogMessageType& LogMessage::getType(void) const noexcept {
    return type;
}

bool LogMessage::shouldMessageBeDisplayed(void) noexcept {
    return timer.getTotalTimeElapsed().count() < timeAllowedForDisplay;
}