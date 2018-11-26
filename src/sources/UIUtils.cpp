#include <UIUtils.hpp>

void UIUtils::translate(UIElement& element, const vec2& t) noexcept {
    SceneUtils::translate(element, t.x, t.y, 0.f);
}

void UIUtils::translate(UIElement& element, const float &tX, const float& tY) noexcept {
    SceneUtils::translate(element, tX, tY, 0.f);
}

void UIUtils::translate(shared_ptr<UIElement>& element, const vec2& t) noexcept {
    SceneUtils::translate(*element.get(), t.x, t.y, 0.f);
}

void UIUtils::translate(shared_ptr<UIElement>& element, const float &tX, const float& tY) noexcept {
    SceneUtils::translate(*element.get(), tX, tY, 0.f);
}

void UIUtils::rotate(UIElement& element, const float& degrees) noexcept {
    SceneUtils::rotate(element, 0.f, 0.f, degrees);
}

void UIUtils::rotate(shared_ptr<UIElement>& element, const float& degrees) noexcept {
    SceneUtils::rotate(*element.get(), 0.f, 0.f, degrees);
}

void UIUtils::orbit(UIElement& element, const float& degrees) noexcept {
    SceneUtils::orbit(element, 0.f, 0.f, degrees);
}

void UIUtils::orbit(shared_ptr<UIElement>& element, const float& degrees) noexcept {
    SceneUtils::orbit(*element.get(), 0.f, 0.f, degrees);
}

void UIUtils::scale(UIElement& element, const vec2& s) noexcept {
    SceneUtils::scale(element, s.x, s.y, 1.f);
}

void UIUtils::scale(UIElement& element, const float& sX, const float& sY) noexcept {
    SceneUtils::scale(element, sX, sY, 1.f);
}

void UIUtils::scale(shared_ptr<UIElement>& element, const vec2& s) noexcept {
    SceneUtils::scale(*element.get(), s.x, s.y, 1.f);
}

void UIUtils::scale(shared_ptr<UIElement>& element, const float& sX, const float& sY) noexcept {
    SceneUtils::scale(*element.get(), sX, sY, 1.f);
}

void UIUtils::draw(const shared_ptr<UIElement>& element, const mat4& ProjectionViewMatrix) noexcept {
    if (element->getShader() != nullptr) {
        mat4 model = element->getTransform().getMatrix();

        element->getShader()->use();
        element->getShader()->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        element->getShader()->setMat4("model", value_ptr(model));
        element->getShader()->setVec4("fillColor", value_ptr(element->getState().getFillColor()));

        glBindVertexArray(element->getVAO());
        vector<Vertex> vertices = element->getState().getUIVertices();
        glBindBuffer(GL_ARRAY_BUFFER, element->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
        glBindVertexArray(0);

        TextRenderingSystem::renderText(
            element->getTextShader(),
            element->getState(),
            ProjectionViewMatrix,
            model
        );
    }
}

void UIUtils::setWidth(shared_ptr<UIElement>& element, const GLfloat& width) noexcept {
    element->getState().setWidth(width);
}

void UIUtils::setHeight(shared_ptr<UIElement>& element, const GLfloat& height) noexcept {
    element->getState().setHeight(height);
}

void UIUtils::setFillColor(shared_ptr<UIElement>& element, const vec4& fillColor) noexcept {
    element->getState().setFillColor(fillColor);
}

void UIUtils::setTextColor(shared_ptr<UIElement>& element, const vec4& textColor) noexcept {
    element->getState().setTextColor(textColor);
}

void UIUtils::setText(shared_ptr<UIElement>& element, const string& text) noexcept {
    element->getState().setText(text);
}

void UIUtils::setFontKey(shared_ptr<UIElement>& element, const FontKey& key) noexcept {
    element->getState().setFontKey(key);
}

void UIUtils::setFontSize(shared_ptr<UIElement>& element, const GLfloat& fontSize) noexcept {
    element->getState().setFontSize(fontSize);
}

void UIUtils::setTextAlign(shared_ptr<UIElement>& element, const TextAlign& textAlign) noexcept {
    element->getState().setTextAlign(textAlign);
}

void UIUtils::setTextVerticalAlign(shared_ptr<UIElement>& element, const TextVerticalAlign& textVerticalAlign) noexcept {
    element->getState().setTextVerticalAlign(textVerticalAlign);
}

void UIUtils::setWordWrap(shared_ptr<UIElement>& element, const bool& wordWrap) noexcept {
    element->getState().setWordWrap(wordWrap);
}

void UIUtils::setZIndex(shared_ptr<UIElement>& element, const GLfloat& zIndex) noexcept {
    element->getState().setZIndex(zIndex);

    UIElement::ON_ZINDEX_CHANGE payload = {
        element
    };
    element->getManager().emit<UIElement::ON_ZINDEX_CHANGE>(payload);
}