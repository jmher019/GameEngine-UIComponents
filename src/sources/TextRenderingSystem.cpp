#include <TextRenderingSystem.hpp>

vector<Vertex> TextRenderingSystem::vertices = vector<Vertex>(TextRenderingSystem::CHARACTERS_PER_LINE * 6, Vertex());
GLuint TextRenderingSystem::fontVAO = 0;
GLuint TextRenderingSystem::fontVBO = 0;
bool TextRenderingSystem::initialized = false;

void TextRenderingSystem::startUp(void) {
    if (!initialized) {
        glGenVertexArrays(1, &TextRenderingSystem::fontVAO);
        glGenBuffers(1, &TextRenderingSystem::fontVBO);
        glBindVertexArray(TextRenderingSystem::fontVAO);
        glBindBuffer(GL_ARRAY_BUFFER, TextRenderingSystem::fontVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
        
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

        initialized = true;
    }
}

void TextRenderingSystem::shutDown(void) {
    if (initialized) {
        glDeleteVertexArrays(1, &TextRenderingSystem::fontVAO);
        glDeleteBuffers(1, &TextRenderingSystem::fontVBO);

        initialized = false;
    }
}

void TextRenderingSystem::renderLine(
    Character*& characters,
    const shared_ptr<Shader>& textShader,
    const UIState& state,
    const string& text,
    const vec2& textSize,
    const vec4& textColor,
    const GLfloat& minY,
    const GLfloat& line,
    const GLfloat& linesInText,
    const GLfloat& lineSpacing,
    const GLfloat& z,
    const GLfloat& scale
) {
    textShader->setVec4("textColor", value_ptr(textColor));
    GLfloat x = 0;
    GLfloat y = 0;
    GLuint charactersInCurrentLine = 0;
    GLuint firstTriangleIndex1 = 0;
    GLuint firstTriangleIndex2 = 1;
    GLuint firstTriangleIndex3 = 2;

    GLuint secondTriangleIndex1 = 3;
    GLuint secondTriangleIndex2 = 4;
    GLuint secondTriangleIndex3 = 5;

    vec2 offset = vec2();
    switch (state.getTextAlign()) {
    case TextAlign::LEFT: {
        offset.x = -state.getWidth() * 0.5f;
        break;
    }
    case TextAlign::CENTER: {
        offset.x = -textSize.x * 0.5f;
        break;
    }
    case TextAlign::RIGHT: {
        offset.x = -textSize.x + state.getWidth() * 0.5f;
        break;
    }
    }

    switch (state.getTextVerticalAlign()) {
        case TextVerticalAlign::TOP: {
            offset.y = -minY - state.getHeight() * 0.5f + line * lineSpacing;
            break;
        }
        case TextVerticalAlign::CENTER: {
            offset.y = (lineSpacing - linesInText) * 0.5f + line * lineSpacing;
            break;
        }
        case TextVerticalAlign::BOTTOM: {
            offset.y = state.getHeight() * 0.5f - line * lineSpacing;
            break;
        }
    }

    for (auto iterator = text.begin(); iterator != text.end(); ++iterator) {
        const Character& character = characters[*iterator];

        GLfloat xpos = x + character.bearing.x * scale + offset.x;
        GLfloat ypos = y - character.bearing.y * scale + offset.y;

        GLfloat charWidth = character.size.x * scale;
        GLfloat charHeight = character.size.y * scale;

        // Update VBO for each character
        vertices[firstTriangleIndex1] = {
            vec3(xpos, ypos + charHeight, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(character.texCoordUpperLeft.x, character.texCoordLowerRight.y)
        };

        vertices[firstTriangleIndex2] = {
            vec3(xpos, ypos, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(character.texCoordUpperLeft.x, character.texCoordUpperLeft.y)
        };

        vertices[firstTriangleIndex3] = {
            vec3(xpos + charWidth, ypos, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(character.texCoordLowerRight.x, character.texCoordUpperLeft.y)
        };

        vertices[secondTriangleIndex1] = {
            vec3(xpos, ypos + charHeight, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(character.texCoordUpperLeft.x, character.texCoordLowerRight.y)
        };

        vertices[secondTriangleIndex2] = {
            vec3(xpos + charWidth, ypos, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(character.texCoordLowerRight.x, character.texCoordUpperLeft.y)
        };

        vertices[secondTriangleIndex3] = {
            vec3(xpos + charWidth, ypos + charHeight, z),
            vec3(0.f, 0.f, 1.f),
            vec3(0.f, 0.f, 0.f),
            vec2(character.texCoordLowerRight.x, character.texCoordLowerRight.y)
        };

        ++charactersInCurrentLine;
        firstTriangleIndex1 += 6;
        firstTriangleIndex2 = firstTriangleIndex1 + 1;
        firstTriangleIndex3 = firstTriangleIndex2 + 1;

        secondTriangleIndex1 = firstTriangleIndex3 + 1;
        secondTriangleIndex2 = secondTriangleIndex1 + 1;
        secondTriangleIndex3 = secondTriangleIndex2 + 1;

        x += static_cast<GLfloat>(character.advance) * scale;
    }

    if (charactersInCurrentLine) {
        const GLuint numVerticesModified = charactersInCurrentLine * 6;

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, TextRenderingSystem::fontVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, numVerticesModified * sizeof(Vertex), &vertices[0]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, numVerticesModified);
    }
}

void TextRenderingSystem::renderText(
    const shared_ptr<Shader>& textShader,
    const UIState& state,
    const mat4& ProjectionViewMatrix,
    const mat4& model
) {
    GLfloat scale = state.getFontSize() / static_cast<GLfloat>(FontSystem::getFontSize());

    if (!FontSystem::isInitialized()) {
        cout << "FreeType library has not been initialized!" << endl;
        return;
    }

    if (!FontSystem::getFaceMap().count(state.getFontKey())) {
        cout << "Font face has not been loaded." << endl;
        return;
    }

    if (!FontSystem::getFaceTextureMap().count(state.getFontKey())) {
        cout << "Font bitmap has not been generated." << endl;
        return;
    }

    if (!initialized) {
        cout << "Text Rendering System has not been initialized" << endl;
        return;
    }

    if (state.getText().empty()) {
        return;
    }

    if (textShader != nullptr) {
        textShader->use();
        textShader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        textShader->setMat4("model", value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(TextRenderingSystem::fontVAO);

        glBindTexture(GL_TEXTURE_2D, FontSystem::getFaceTextureMap().at(state.getFontKey()));

        Character* characters = FontSystem::getFaceGlyphMap().at(state.getFontKey());
        string::const_iterator startIt;
        string::const_iterator it;
        vector<GLfloat> minYs;
        vector<vec2> textSizes;
        vector<string> texts;
        for (it = state.getText().begin(); it != state.getText().end();) {
            GLuint charactersInCurrentLine = 0;
            GLfloat x = 0;
            GLfloat y = 0;
            GLfloat minX = x;
            GLfloat minY = y;
            GLfloat maxX = x;
            GLfloat maxY = y;

            startIt = it;
            for (; it != state.getText().end(); ++it) {
                if (*it == '\n') {
                    ++it;
                    break;
                }

                const Character& character = characters[*it];

                GLfloat xpos = x + character.bearing.x * scale;
                GLfloat ypos = y - character.bearing.y * scale;

                GLfloat charWidth = character.size.x * scale;
                GLfloat charHeight = character.size.y * scale;

                if (maxX < xpos + charWidth) {
                    maxX = xpos + charWidth;
                }

                if (minY > ypos) {
                    minY = ypos;
                }

                if (maxY < ypos + charHeight) {
                    maxY = ypos + charHeight;
                }

                if (state.getWordWrap() && state.getWidth() < xpos + charWidth - minX) {
                    break;
                }

                ++charactersInCurrentLine;
                if (charactersInCurrentLine == CHARACTERS_PER_LINE) {
                    ++it;
                    break;
                }

                x += static_cast<GLfloat>(character.advance) * scale;
            }
            
            minYs.push_back(minY);
            textSizes.push_back(vec2(maxX - minX, maxY - minY));
            texts.push_back(string(startIt, it));
        }


        GLfloat lineSpacing = static_cast<GLfloat>(FontSystem::getFaceMap().at(state.getFontKey())->size->metrics.height >> 6) * scale;
        GLfloat lines = 0;
        GLfloat z = -clamp(state.getZIndex(), 0.f, UIState::MAX_Z_INDEX) / UIState::MAX_Z_INDEX;
        GLfloat linesInText = static_cast<GLfloat>(texts.size());
        for (size_t i = 0; i < texts.size(); i++) {
            const GLfloat& minY = minYs[i];
            const vec2& textSize = textSizes[i];
            const string& text = texts[i];

            renderLine(
                characters,
                textShader,
                state,
                texts[i],
                textSizes[i],
                state.getTextColor(),
                minYs[i],
                lines,
                linesInText,
                lineSpacing,
                z,
                scale
            );

            ++lines;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void TextRenderingSystem::renderText(
    const shared_ptr<Shader>& textShader,
    const UIState& state,
    const vector<LogMessage>& logMessages,
    const mat4& ProjectionViewMatrix,
    const mat4& model
) {
    GLfloat scale = state.getFontSize() / static_cast<GLfloat>(FontSystem::getFontSize());

    if (!FontSystem::isInitialized()) {
        cout << "FreeType library has not been initialized!" << endl;
        return;
    }

    if (!FontSystem::getFaceMap().count(state.getFontKey())) {
        cout << "Font face has not been loaded." << endl;
        return;
    }

    if (!FontSystem::getFaceTextureMap().count(state.getFontKey())) {
        cout << "Font bitmap has not been generated." << endl;
        return;
    }

    if (!initialized) {
        cout << "Text Rendering System has not been initialized" << endl;
        return;
    }

    if (logMessages.empty()) {
        return;
    }

    if (textShader != nullptr) {
        textShader->use();
        textShader->setMat4("PVM", value_ptr(ProjectionViewMatrix * model));
        textShader->setMat4("model", value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(TextRenderingSystem::fontVAO);

        glBindTexture(GL_TEXTURE_2D, FontSystem::getFaceTextureMap().at(state.getFontKey()));

        Character* characters = FontSystem::getFaceGlyphMap().at(state.getFontKey());
        string::const_iterator startIt;
        string::const_iterator it;
        vector<GLfloat> minYs;
        vector<vec2> textSizes;
        vector<string> texts;
        vector<vec4> textColors;
        for (int messageIndex = static_cast<int>(logMessages.size()) - 1; messageIndex >= 0; --messageIndex) {
            const LogMessage& logMessage = logMessages[messageIndex];
            const string& text = logMessage.getMessage();

            for (it = text.begin(); it != text.end();) {
                GLuint charactersInCurrentLine = 0;
                GLfloat x = 0;
                GLfloat y = 0;
                GLfloat minX = x;
                GLfloat minY = y;
                GLfloat maxX = x;
                GLfloat maxY = y;

                startIt = it;
                for (; it != text.end(); ++it) {
                    if (*it == '\n') {
                        ++it;
                        break;
                    }

                    const Character& character = characters[*it];

                    GLfloat xpos = x + character.bearing.x * scale;
                    GLfloat ypos = y - character.bearing.y * scale;

                    GLfloat charWidth = character.size.x * scale;
                    GLfloat charHeight = character.size.y * scale;

                    if (maxX < xpos + charWidth) {
                        maxX = xpos + charWidth;
                    }

                    if (minY > ypos) {
                        minY = ypos;
                    }

                    if (maxY < ypos + charHeight) {
                        maxY = ypos + charHeight;
                    }

                    if (state.getWordWrap() && state.getWidth() < xpos + charWidth - minX) {
                        break;
                    }

                    ++charactersInCurrentLine;
                    if (charactersInCurrentLine == CHARACTERS_PER_LINE) {
                        ++it;
                        break;
                    }

                    x += static_cast<GLfloat>(character.advance) * scale;
                }

                minYs.push_back(minY);
                textSizes.push_back(vec2(maxX - minX, maxY - minY));
                texts.push_back(string(startIt, it));
                textColors.push_back(logMessage.getTextColor());
            }
        }

        GLfloat lineSpacing = static_cast<GLfloat>(FontSystem::getFaceMap().at(state.getFontKey())->size->metrics.height >> 6) * scale;
        GLfloat lines = 0;
        GLfloat z = -clamp(state.getZIndex(), 0.f, UIState::MAX_Z_INDEX) / UIState::MAX_Z_INDEX;
        GLfloat linesInText = static_cast<GLfloat>(texts.size());

        switch (state.getTextVerticalAlign()) {
            case TextVerticalAlign::BOTTOM: {
                for (size_t i = 0; i < texts.size(); i++) {
                    const GLfloat& minY = minYs[i];
                    const vec2& textSize = textSizes[i];
                    const string& text = texts[i];

                    renderLine(
                        characters,
                        textShader,
                        state,
                        texts[i],
                        textSizes[i],
                        textColors[i],
                        minYs[i],
                        lines,
                        linesInText,
                        lineSpacing,
                        z,
                        scale
                    );

                    ++lines;
                }
                break;
            }
            case TextVerticalAlign::CENTER: {}
            case TextVerticalAlign::TOP: {
                GLfloat fullHeight = static_cast<GLfloat>(texts.size()) * lineSpacing;
                
                int finalSize = static_cast<int>(texts.size());
                if (fullHeight > state.getHeight()) {
                    finalSize = static_cast<int>(state.getHeight() / lineSpacing);
                }
                
                for (int i = 0; i < finalSize; ++i, ++lines) {
                    const GLfloat& minY = minYs[i];
                    const vec2& textSize = textSizes[i];
                    const string& text = texts[i];

                    renderLine(
                        characters,
                        textShader,
                        state,
                        texts[i],
                        textSizes[i],
                        textColors[i],
                        minYs[i],
                        lines,
                        linesInText,
                        lineSpacing,
                        z,
                        scale
                    );
                }
                break;
            }
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}