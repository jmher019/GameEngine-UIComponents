#include <FontSystem.hpp>

FT_Library FontSystem::library;
unordered_map<FontKey, FT_Face> FontSystem::faceMap;
unordered_map<FontKey, GLuint> FontSystem::faceTextureMap;
unordered_map<FontKey, Character*> FontSystem::faceGlyphMap;
GLuint FontSystem::fontSize = 48;
bool FontSystem::initialized = false;

void FontSystem::startUp(void) {
    if (!initialized) {
        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_Error error = FT_Init_FreeType(&FontSystem::library);

        if (error) {
            cout << "Failure to initialize FreeType library" << endl;
            return;
        }

        initialized = true;
    }
}

void FontSystem::shutDown(void) {
    if (initialized) {
        // Deallocate textures
        for (auto it = FontSystem::faceTextureMap.begin(); it != FontSystem::faceTextureMap.end(); ++it) {
            glDeleteTextures(1, &(*it).second);
        }
        FontSystem::faceTextureMap.clear();

        // Deallocate Character struct arrays
        for (auto it = FontSystem::faceGlyphMap.begin(); it != FontSystem::faceGlyphMap.end(); ++it) {
            delete[] (*it).second;
            (*it).second = nullptr;
        }
        FontSystem::faceGlyphMap.clear();

        // Discard faces
        for (auto it = FontSystem::faceMap.begin(); it != FontSystem::faceMap.end(); ++it) {
            FT_Done_Face((*it).second);
        }
        FontSystem::faceMap.clear();

        // Clean up the library
        FT_Done_FreeType(FontSystem::library);
    
        initialized = false;
    }
}

void FontSystem::loadFace(const string& fileName, const FontKey& key) {
    if (!initialized) {
        cout << "FreeType library has not been initialized!" << endl;
        return;
    }
    
    if (FontSystem::faceMap.count(key)) {
        cout << "Face has already been loaded!" << endl;
        return;
    }

    auto insertedPair = FontSystem::faceMap.insert(pair<FontKey, FT_Face>(key, FT_Face()));

    if (!insertedPair.second) {
        cout << "Failed to insert font face to face map." << endl;
    }
    else {
        FT_Face& face = FontSystem::faceMap[key];

        FT_Error error = FT_New_Face(
            FontSystem::library,
            fileName.c_str(),
            0,
            &face
        );

        if (error == FT_Err_Unknown_File_Format) {
            cout << "The font file could be opened and read, but it appears that its font formate is unsupported." << endl;
        }
        else if (error) {
            cout << "Font file could not be opened, read, or it is broken." << endl;
        }

        if (FT_Set_Pixel_Sizes(face, 0, FontSystem::fontSize)) {
            cout << "Failed to set fontsize." << endl;
        }

        createTextureBitmap(key);
    }
}

const bool& FontSystem::isInitialized(void) noexcept {
    return FontSystem::initialized;
}

FT_GlyphSlot FontSystem::loadGlyph(const FontKey& key, const unsigned char& glyph) {
    if (!FontSystem::faceMap.count(key)) {
        cout << "Font face has not been loaded." << endl;
        return nullptr;
    }

    FT_Face& face = FontSystem::faceMap[key];

    if (FT_Load_Char(face, glyph, FT_LOAD_RENDER)) {
        cout << "Failed to load glyph" << endl;
        return nullptr;
    }

    return face->glyph;
}

void FontSystem::createTextureBitmap(const FontKey& key) {
    if (!faceMap.count(key)) {
        cout << "Font face has not been loaded." << endl;
        return;
    }

    if (faceTextureMap.count(key)) {
        cout << "Face texture has already been generated!" << endl;
        return;
    }

    faceTextureMap.insert(pair<FontKey, GLuint>(key, 0));
    GLuint& id = faceTextureMap[key];

    // create bitmap buffer
    constexpr size_t blockSize = 64;
    constexpr size_t blocksPerRow = 8;
    constexpr size_t blocksPerColumn = 16;
    constexpr size_t width = blockSize * blocksPerRow;
    constexpr size_t height = blockSize * blocksPerColumn;
    
    unsigned char buffer[blockSize * blockSize * ASCII_CHARACTERS];
    constexpr size_t fullSize = blockSize * blockSize * ASCII_CHARACTERS;
    for (size_t i = 0; i < fullSize; i++) {
        buffer[i] = static_cast<unsigned char>(0);
    }

    size_t bufferOffset = 0;
    
    constexpr GLfloat oneOverWidth = 1.f / static_cast<GLfloat>(width);
    constexpr GLfloat oneOverHeight = 1.f / static_cast<GLfloat>(height);
    Character* characters = new Character[ASCII_CHARACTERS];
    for (size_t blockY = 0, i = 0; blockY < blocksPerColumn; blockY++, bufferOffset += width * blockSize) {
        size_t currentBlockOffset = bufferOffset;
        for (size_t blockX = 0; blockX < blocksPerRow; blockX++, currentBlockOffset += blockSize, i++) {
            size_t currentSubOffset = currentBlockOffset;

            FT_GlyphSlot glyph = loadGlyph(key, static_cast<unsigned char>(i));

            for (GLuint y = 0, offset = 0; y < glyph->bitmap.rows; y++, offset += glyph->bitmap.width, currentSubOffset += width) {
                memcpy(&buffer[currentSubOffset], &glyph->bitmap.buffer[offset], glyph->bitmap.width);
            }

            characters[i] = {
                id,
                vec2(glyph->bitmap.width, glyph->bitmap.rows),
                vec2(glyph->bitmap_left, glyph->bitmap_top),
                (glyph->advance.x >> 6), // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
                vec2(
                    static_cast<GLfloat>(blockX * blockSize) * oneOverWidth,
                    static_cast<GLfloat>(blockY * blockSize) * oneOverHeight
                ),
                vec2(
                    static_cast<GLfloat>(blockX * blockSize + glyph->bitmap.width) * oneOverWidth,
                    static_cast<GLfloat>(blockY * blockSize + glyph->bitmap.rows) * oneOverHeight
                )
            };
        }
    }

    faceGlyphMap.insert(pair<FontKey, Character*>(key, characters));

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        width,
        height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        &buffer
    );

    for (size_t i = 0; i < ASCII_CHARACTERS; i++) {
        characters[i].textureId = id;
    }

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

constexpr size_t FontSystem::getNumberOfStoredAsciiCharacters(void) noexcept {
    return FontSystem::ASCII_CHARACTERS;
}

const GLuint& FontSystem::getFontSize(void) noexcept {
    return FontSystem::fontSize;
}

const unordered_map<FontKey, FT_Face>& FontSystem::getFaceMap(void) noexcept {
    return FontSystem::faceMap;
}

const unordered_map<FontKey, GLuint>& FontSystem::getFaceTextureMap(void) noexcept {
    return FontSystem::faceTextureMap;
}

const unordered_map<FontKey, Character*>& FontSystem::getFaceGlyphMap(void) noexcept {
    return FontSystem::faceGlyphMap;
}