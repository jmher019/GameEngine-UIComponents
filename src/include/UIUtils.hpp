#ifndef UI_UTILS_HPP
#define UI_UTILS_HPP

#include <SceneUtils.hpp>
#include <UIElement.hpp>

#include <glm\gtc\type_ptr.hpp>

class UIUtils {
public:
    UIUtils(void) = delete;
    UIUtils(const UIUtils& uiUtils) = delete;
    UIUtils(UIUtils&& uiUtils) = delete;
    UIUtils& operator=(const UIUtils& uiUtils) = delete;
    UIUtils& operator=(UIUtils&& uiUtils) = delete;

    static void translate(UIElement& element, const vec2& t) noexcept;
    static void translate(UIElement& element, const float& tX, const float& tY) noexcept;
    static void translate(shared_ptr<UIElement>& element, const vec2& t) noexcept;
    static void translate(shared_ptr<UIElement>& element, const float& tX, const float& tY) noexcept;

    static void rotate(UIElement& element, const float& degrees) noexcept;
    static void rotate(shared_ptr<UIElement>& element, const float& degrees) noexcept;

    static void orbit(UIElement& element, const float& degrees) noexcept;
    static void orbit(shared_ptr<UIElement>& element, const float& degrees) noexcept;

    static void scale(UIElement& element, const vec2& s) noexcept;
    static void scale(UIElement& element, const float& sX, const float& sY) noexcept;
    static void scale(shared_ptr<UIElement>& element, const vec2& s) noexcept;
    static void scale(shared_ptr<UIElement>& element, const float& sX, const float& sY) noexcept;

    static void draw(const shared_ptr<UIElement>& element, const mat4& ProjectionViewMatrix) noexcept;

    static void setWidth(shared_ptr<UIElement>& element, const GLfloat& width) noexcept;

    static void setHeight(shared_ptr<UIElement>& element, const GLfloat& height) noexcept;

    static void setFillColor(shared_ptr<UIElement>& element, const vec4& fillColor) noexcept;

    static void setTextColor(shared_ptr<UIElement>& element, const vec4& textColor) noexcept;

    static void setText(shared_ptr<UIElement>& element, const string& text) noexcept;

    static void setFontKey(shared_ptr<UIElement>& element, const FontKey& key) noexcept;

    static void setFontSize(shared_ptr<UIElement>& element, const GLfloat& fontSize) noexcept;

    static void setTextAlign(shared_ptr<UIElement>& element, const TextAlign& textAlign) noexcept;

    static void setTextVerticalAlign(shared_ptr<UIElement>& element, const TextVerticalAlign& textVerticalAlign) noexcept;

    static void setWordWrap(shared_ptr<UIElement>& element, const bool& wordWrap) noexcept;

    static void setZIndex(shared_ptr<UIElement>& element, const GLfloat& zIndex) noexcept;
};

#endif // !UI_UTILS_HPP
