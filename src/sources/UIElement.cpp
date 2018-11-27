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
    SceneObject::draw(ProjectionViewMatrix);
}

const GLuint& UIElement::getVAO(void) const noexcept {
    return VAO;
}

const GLuint& UIElement::getVBO(void) const noexcept {
    return VBO;
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

UIState& UIElement::getState(void) noexcept {
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

unsigned long UIElement::subscribeToOnZindexChange(function<void(const ON_ZINDEX_CHANGE&)> callable) {
    return getManager().subscribe<ON_ZINDEX_CHANGE>(callable);
}

unsigned long UIElement::subscribeToOnClick(function<void(const ON_CLICK&)> callable) {
    return getManager().subscribe<ON_CLICK>(callable);
}

bool UIElement::operator()(const shared_ptr<UIElement>& lhs, const shared_ptr<UIElement>& rhs) const {
    if (lhs->getState().getZIndex() == rhs->getState().getZIndex()) {
        return lhs.get() < rhs.get();
    }

    return lhs->getState().getZIndex() < rhs->getState().getZIndex();
}

void UIElement::onClick(const double& x, const double& y) noexcept {
    ON_CLICK payload = {
        this,
        x,
        y
    };

    getManager().emit(payload);
}