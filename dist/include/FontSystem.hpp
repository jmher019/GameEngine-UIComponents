#ifndef FONT_SYSTEM_HPP
#define FONT_SYSTEM_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Vertex.hpp>
#include <Shader.hpp>

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

#include <glm\gtc\type_ptr.hpp>

using namespace std;

enum class FontKey {
    ARIAL
};

struct Character {
public:
    GLuint textureId;
    vec2 size;
    vec2 bearing;
    GLint advance;
    vec2 texCoordUpperLeft;
    vec2 texCoordLowerRight;
};

class FontSystem {
private:
    static constexpr size_t ASCII_CHARACTERS = 128;
    static FT_Library library;
    static unordered_map<FontKey, FT_Face> faceMap;
    static unordered_map<FontKey, GLuint> faceTextureMap;
    static unordered_map<FontKey, Character*> faceGlyphMap;
    static GLuint fontSize;
    static bool initialized;

    static FT_GlyphSlot loadGlyph(const FontKey& key, const unsigned char& glyph);

    static void createTextureBitmap(const FontKey& key);

public:
    static void startUp(void);

    static void shutDown(void);

    static void loadFace(const string& fileName, const FontKey& key);

    static const bool& isInitialized(void) noexcept;

    static constexpr size_t getNumberOfStoredAsciiCharacters(void) noexcept;

    static const GLuint& getFontSize(void) noexcept;

    static const unordered_map<FontKey, FT_Face>& getFaceMap(void) noexcept;

    static const unordered_map<FontKey, GLuint>& getFaceTextureMap(void) noexcept;

    static const unordered_map<FontKey, Character*>& getFaceGlyphMap(void) noexcept;
};

#endif // !FONT_SYSTEM_HPP
