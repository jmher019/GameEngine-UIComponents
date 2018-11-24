#include <UIElement.hpp>

UIElement::UIElement(const string& name, const Transform& transform):
    SceneObject(name, transform) {
    initialize();
}

UIElement::UIElement(const UIElement& element) :
    SceneObject(element.name, element.transform),
    state(element.state) {
    initialize();
}

UIElement::UIElement(UIElement&& element):
    SceneObject(move(element.name), move(element.transform)),
    VAO(move(element.VAO)),
    VBO(move(element.VBO)),
    state(move(element.state)) {
}

UIElement::~UIElement(void) {
    deallocate();
}

UIElement& UIElement::operator=(const UIElement& element) noexcept {
    name = element.name;
    transform = element.transform;
    state = element.state;

    deallocate();
    initialize();

    return *this;
}

UIElement& UIElement::operator=(UIElement&& element) noexcept {
    deallocate();

    VAO = move(element.VAO);
    VBO = move(element.VBO);
    name = move(element.name);
    transform = move(element.transform);
    state = move(element.state);

    return *this;
}

void UIElement::initialize(void) noexcept {
    if (!VAO) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, state.getUIVertices().size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void UIElement::deallocate() noexcept {
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        VAO = 0;
        VBO = 0;
    }
}

void UIElement::draw(const mat4& ProjectionViewMatrix) const {
    if (shader != nullptr) {
        mat4 model = transform.getMatrix();

        shader->use();
        shader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        shader->setMat4("model", value_ptr(model));
        shader->setVec4("fillColor", value_ptr(state.getFillColor()));

        glBindVertexArray(getVAO());
        vector<Vertex> vertices = state.getUIVertices();
        glBindBuffer(GL_ARRAY_BUFFER, getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
        glBindVertexArray(0);

        TextRenderingSystem::renderText(
            textShader,
            state,
            ProjectionViewMatrix,
            model
        );
    }

    SceneObject::draw(ProjectionViewMatrix);
}

void UIElement::translate(const float& tX, const float& tY) noexcept {
    SceneObject::translate(tX, tY, 0.f);
}

void UIElement::rotate(const float& degrees) noexcept {
    SceneObject::rotate(0.f, 0.f, degrees);
}

void UIElement::orbit(const float& degrees) noexcept {
    SceneObject::orbit(0.f, 0.f, degrees);
}

const GLuint& UIElement::getVAO(void) const noexcept {
    return VAO;
}

const GLuint& UIElement::getVBO(void) const noexcept {
    return VBO;
}

void UIElement::setWidth(const GLfloat& width) noexcept {
    state.setWidth(width);
}

const GLfloat& UIElement::getWidth(void) const noexcept {
    return state.getWidth();
}

void UIElement::setHeight(const GLfloat& height) noexcept {
    state.setHeight(height);
}

const GLfloat& UIElement::getHeight(void) const noexcept {
    return state.getHeight();
}

void UIElement::setFillColor(const vec4& fillColor) noexcept {
    state.setFillColor(fillColor);
}

const vec4& UIElement::getFillColor(void) const noexcept {
    return state.getFillColor();
}

void UIElement::setTextColor(const vec4& textColor) noexcept {
    this->state.setTextColor(textColor);
}

const vec4& UIElement::getTextColor(void) const noexcept {
    return this->state.getTextColor();
}

void UIElement::setText(const string& text) noexcept {
    this->state.setText(text);
}

const string& UIElement::getText(void) const noexcept {
    return this->state.getText();
}

void UIElement::setFontKey(const FontKey& key) noexcept {
    this->state.setFontKey(key);
}

const FontKey& UIElement::getFontKey(void) const noexcept {
    return this->state.getFontKey();
}

void UIElement::setFontSize(const GLfloat& fontSize) noexcept {
    this->state.setFontSize(fontSize);
}

const GLfloat& UIElement::getFontSize(void) const noexcept {
    return this->state.getFontSize();
}

void UIElement::setTextAlign(const TextAlign& textAlign) noexcept {
    this->state.setTextAlign(textAlign);
}

const TextAlign& UIElement::getTextAlign(void) const noexcept {
    return this->state.getTextAlign();
}

void UIElement::setTextVerticalAlign(const TextVerticalAlign& textVerticalAlign) noexcept {
    this->state.setTextVerticalAlign(textVerticalAlign);
}

const TextVerticalAlign& UIElement::getTextVerticalAlign(void) const noexcept {
    return this->state.getTextVerticalAlign();
}

void UIElement::setWordWrap(const bool& wordWrap) noexcept {
    this->state.setWordWrap(wordWrap);
}

const bool& UIElement::getWordWrap(void) const noexcept {
    return this->state.getWordWrap();
}

void UIElement::setZIndex(const GLfloat& zIndex) noexcept {
    this->state.setZIndex(zIndex);

    for (auto& child : children) {
        UIElement* uiElement = nullptr;
        if ((uiElement = dynamic_cast<UIElement*>(child.get())) != nullptr) {
            uiElement->setZIndex(zIndex);
        }
    }
}

const GLfloat& UIElement::getZIndex(void) const noexcept {
    return this->state.getZIndex();
}

void UIElement::setShader(const shared_ptr<Shader>& shader) noexcept {
    this->shader = shader;
}

const shared_ptr<Shader>& UIElement::getShader(void) const noexcept {
    return this->shader;
}

void UIElement::setTextShader(const shared_ptr<Shader>& textShader) noexcept {
    this->textShader = textShader;
}

const shared_ptr<Shader>& UIElement::getTextShader(void) const noexcept {
    return this->textShader;
}

const UIState& UIElement::getState(void) const noexcept {
    return this->state;
}

GLboolean UIElement::isWithinElement(const GLfloat& x, const GLfloat& y) const noexcept {
    const GLfloat width = state.getWidth();
    const GLfloat height = state.getHeight();
    GLfloat minX = -width * 0.5f;
    GLfloat maxX = width * 0.5f;
    GLfloat minY = -height * 0.5f;
    GLfloat maxY = height * 0.5f;

    const vec2 point = vec2(transform.getInverseMatrix() * vec4(x, y, 0.f, 1.f));
    return point.x >= minX && point.x <= maxX && point.y >= minY && point.y <= maxY;
}