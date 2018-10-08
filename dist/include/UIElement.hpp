#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <SceneObject.hpp>
#include <Shader.hpp>
#include <TextRenderingSystem.hpp>

#include <glm\gtc\type_ptr.hpp>

class UIElement : public SceneObject {
private:
    GLuint VAO = 0;
    GLuint VBO = 0;
    UIState state;
    shared_ptr<Shader> shader = nullptr;
    shared_ptr<Shader> textShader = nullptr;

    void initialize(void) noexcept;

    void deallocate(void) noexcept;

public:
    UIElement(const string& name = string(""), const Transform& transform = Transform());

    UIElement(const UIElement& element);

    UIElement(UIElement&& element);

    ~UIElement(void);

    UIElement& operator=(const UIElement& other) noexcept;

    UIElement& operator=(UIElement&& other) noexcept;

    void draw(const mat4& ProjectionViewMatrix) const override;

    void translate(const float& tX, const float& tY) noexcept;

    void rotate(const float& degrees) noexcept;

    void orbit(const float& degrees) noexcept;

    const GLuint& getVAO(void) const noexcept;

    const GLuint& getVBO(void) const noexcept;

    void setWidth(const GLfloat& width) noexcept;

    const GLfloat& getWidth() const noexcept;

    void setHeight(const GLfloat& height) noexcept;

    const GLfloat& getHeight() const noexcept;

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

    void setShader(const shared_ptr<Shader>& shader) noexcept;

    const shared_ptr<Shader>& getShader(void) const noexcept;

    void setTextShader(const shared_ptr<Shader>& textShader) noexcept;

    const shared_ptr<Shader>& getTextShader(void) const noexcept;

    const UIState& getState(void) const noexcept;
};

#endif // !UI_ELEMENT_HPP
