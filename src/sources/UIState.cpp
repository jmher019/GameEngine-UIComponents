#include <UIState.hpp>

UIState::UIState(void) {}

UIState::UIState(const UIState& state) {
    setWidth(state.width);
    setHeight(state.height);
    setFillColor(state.fillColor);
    setTextColor(state.textColor);
    setFontSize(state.fontSize);
    setZIndex(state.zIndex);
    setText(state.text);
    setFontKey(state.key);
    setTextAlign(state.textAlign);
    setWordWrap(state.wordWrap);
}

UIState::UIState(UIState&& state) :
    width(move(state.width)),
    height(move(state.height)),
    fillColor(move(state.fillColor)),
    textColor(move(state.textColor)),
    fontSize(move(state.fontSize)),
    zIndex(move(state.zIndex)),
    text(move(state.text)),
    key(move(state.key)),
    textAlign(move(state.textAlign)),
    wordWrap(move(state.wordWrap)) {
}

UIState& UIState::operator=(const UIState& state) noexcept {
    setWidth(state.width);
    setHeight(state.height);
    setFillColor(state.fillColor);
    setTextColor(state.textColor);
    setFontSize(state.fontSize);
    setZIndex(state.zIndex);
    setText(state.text);
    setFontKey(state.key);
    setTextAlign(state.textAlign);
    setWordWrap(state.wordWrap);

    return *this;
}

UIState& UIState::operator=(UIState&& state) noexcept {
    width = move(state.width);
    height = move(state.height);
    fillColor = move(state.fillColor);
    textColor = move(state.textColor);
    fontSize = move(state.fontSize);
    zIndex = move(state.zIndex);
    text = move(state.text);
    key = move(state.key);
    textAlign = move(state.textAlign);
    wordWrap = move(state.wordWrap);

    return *this;
}

void UIState::setWidth(const GLfloat& width) noexcept {
    this->width = width;
}

const GLfloat& UIState::getWidth(void) const noexcept {
    return width;
}

void UIState::setHeight(const GLfloat& height) noexcept {
    this->height = height;
}

const GLfloat& UIState::getHeight(void) const noexcept {
    return height;
}

void UIState::setFillColor(const vec4& fillColor) noexcept {
    this->fillColor = fillColor;
}

const vec4& UIState::getFillColor(void) const noexcept {
    return fillColor;
}

void UIState::setTextColor(const vec4& textColor) noexcept {
    this->textColor = textColor;
}

const vec4& UIState::getTextColor(void) const noexcept {
    return this->textColor;
}

void UIState::setText(const string& text) noexcept {
    this->text = text;
}

const string& UIState::getText(void) const noexcept {
    return this->text;
}

void UIState::setFontKey(const FontKey& key) noexcept {
    this->key = key;
}

const FontKey& UIState::getFontKey(void) const noexcept {
    return this->key;
}

void UIState::setFontSize(const GLfloat& fontSize) noexcept {
    this->fontSize = fontSize;
}

const GLfloat& UIState::getFontSize(void) const noexcept {
    return this->fontSize;
}

void UIState::setTextAlign(const TextAlign& textAlign) noexcept {
    this->textAlign = textAlign;
}

const TextAlign& UIState::getTextAlign(void) const noexcept {
    return this->textAlign;
}

void UIState::setTextVerticalAlign(const TextVerticalAlign& textVerticalAlign) noexcept {
    this->textVerticalAlign = textVerticalAlign;
}

const TextVerticalAlign& UIState::getTextVerticalAlign(void) const noexcept {
    return this->textVerticalAlign;
}

void UIState::setWordWrap(const bool& wordWrap) noexcept {
    this->wordWrap = wordWrap;
}

const bool& UIState::getWordWrap(void) const noexcept {
    return this->wordWrap;
}

void UIState::setZIndex(const GLfloat& zIndex) noexcept {
    this->zIndex = zIndex;
}

const GLfloat& UIState::getZIndex(void) const noexcept {
    return zIndex;
}

vector<Vertex> UIState::getUIVertices(void) const noexcept {
    vector<Vertex> vertices;

    const GLfloat z = -clamp(zIndex, 0.f, MAX_Z_INDEX) / MAX_Z_INDEX;
    vertices.push_back(
        Vertex(
            vec3(-width / 2.f, -height / 2.f, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(0.f, 0.f)
        )
    );

    vertices.push_back(
        Vertex(
            vec3(-width / 2.f, height / 2.f, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(0.f, 1.f)
        )
    );

    vertices.push_back(
        Vertex(
            vec3(width / 2.f, -height / 2.f, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(1.f, 0.f)
        )
    );

    vertices.push_back(
        Vertex(
            vec3(width / 2.f, -height / 2.f, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(1.f, 0.f)
        )
    );

    vertices.push_back(
        Vertex(
            vec3(-width / 2.f, height / 2.f, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(0.f, 1.f)
        )
    );

    vertices.push_back(
        Vertex(
            vec3(width / 2.f, height / 2.f, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(1.f, 1.f)
        )
    );

    return vertices;
}