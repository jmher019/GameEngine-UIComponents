#ifndef TEXT_RENDERING_HPP
#define TEXT_RENDERING_HPP

#include <FontSystem.hpp>
#include <UIState.hpp>
#include <LogMessage.hpp>

class TextRenderingSystem {
private:
    static vector<Vertex> vertices;
    static constexpr size_t CHARACTERS_PER_LINE = 1000;
    static GLuint fontVAO;
    static GLuint fontVBO;
    static bool initialized;

    static void renderLine(
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
    );

public:
    static void startUp(void);

    static void shutDown(void);

    static void renderText(
        const shared_ptr<Shader>& textShader,
        const UIState& state,
        const mat4& ProjectionViewMatrix,
        const mat4& model
    );

    static void renderText(
        const shared_ptr<Shader>& textShader,
        const UIState& state,
        const vector<LogMessage>& logMessages,
        const mat4& ProjectionViewMatrix,
        const mat4& model
    );
};

#endif // !TEXT_RENDERING_HPP
