#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include <SceneObject.hpp>
#include <Shader.hpp>
#include <TextRenderingSystem.hpp>

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
    struct ON_ZINDEX_CHANGE {
        const shared_ptr<UIElement>& element;
    };

    struct ON_CLICK {
        UIElement const* element;
        const double& x;
        const double& y;
    };

public:
    UIElement(const string& name = string(""), const Transform& transform = Transform());

    UIElement(const UIElement& element);

    UIElement(UIElement&& element);

    ~UIElement(void);

    UIElement& operator=(const UIElement& other) noexcept;

    UIElement& operator=(UIElement&& other) noexcept;

    void draw(const mat4& ProjectionViewMatrix) const override;

    const GLuint& getVAO(void) const noexcept;

    const GLuint& getVBO(void) const noexcept;

    void setShader(const shared_ptr<Shader>& shader) noexcept;

    const shared_ptr<Shader>& getShader(void) const noexcept;

    void setTextShader(const shared_ptr<Shader>& textShader) noexcept;

    const shared_ptr<Shader>& getTextShader(void) const noexcept;

    UIState& getState(void) noexcept;

    GLboolean isWithinElement(const GLfloat& x, const GLfloat& y) const noexcept;

    unsigned long subscribeToOnZindexChange(function<void(const ON_ZINDEX_CHANGE&)> callable);

    unsigned long subscribeToOnClick(function<void(const ON_CLICK&)> callable);

    bool operator()(const shared_ptr<UIElement>& lhs, const shared_ptr<UIElement>& rhs) const;

    void onClick(const double& x, const double& y) noexcept;
};

#endif // !UI_ELEMENT_HPP
