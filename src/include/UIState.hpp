#ifndef UI_STATE_HPP
#define UI_STATE_HPP

#include <FontSystem.hpp>

enum class TextAlign {
    LEFT,
    CENTER,
    RIGHT
};

enum class TextVerticalAlign {
    TOP,
    CENTER,
    BOTTOM
};

class UIState {
private:
    GLfloat width = 100;
    GLfloat height = 100;
    vec4 fillColor = vec4(1.f, 1.f, 1.f, 1.f);
    vec4 textColor = vec4(0.f, 0.f, 0.f, 1.f);
    GLfloat fontSize = 48.f;
    GLfloat zIndex = 0.f;
    string text = string("");
    FontKey key = FontKey::ARIAL;
    TextAlign textAlign = TextAlign::LEFT;
    TextVerticalAlign textVerticalAlign = TextVerticalAlign::TOP;
    bool wordWrap = false;

public:
    UIState(void);

    UIState(const UIState& state);

    UIState(UIState&& state);

    UIState& operator=(const UIState& state) noexcept;

    UIState& operator=(UIState&& state) noexcept;

    void setWidth(const GLfloat& width) noexcept;

    const GLfloat& getWidth(void) const noexcept;

    void setHeight(const GLfloat& height) noexcept;

    const GLfloat& getHeight(void) const noexcept;

    void setFillColor(const vec4& fillColor) noexcept;

    const vec4& getFillColor(void) const noexcept;

    void setTextColor(const vec4& textColor) noexcept;

    const vec4& getTextColor(void) const noexcept;

    void setText(const string& text) noexcept;

    const string& getText(void) const noexcept;

    void setFontKey(const FontKey& key) noexcept;

    const FontKey& getFontKey(void) const noexcept;

    void setFontSize(const GLfloat& fontSize) noexcept;

    const GLfloat& getFontSize(void) const noexcept;

    void setTextAlign(const TextAlign& textAlign) noexcept;

    const TextAlign& getTextAlign(void) const noexcept;

    void setTextVerticalAlign(const TextVerticalAlign& textVerticalAlign) noexcept;

    const TextVerticalAlign& getTextVerticalAlign(void) const noexcept;

    void setWordWrap(const bool& wordWrap) noexcept;

    const bool& getWordWrap(void) const noexcept;

    void setZIndex(const GLfloat& zIndex) noexcept;

    const GLfloat& getZIndex(void) const noexcept;

    vector<Vertex> getUIVertices(void) const noexcept;

    static constexpr GLfloat MAX_Z_INDEX = 1e6f;
};

#endif // !UI_STATE_HPP
