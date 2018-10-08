#ifndef BMP_GENERATOR_HPP
#define BMP_GENERATOR_HPP

#include <fstream>

using namespace std;

class BMPGenerator {
private:
    unsigned short width;
    unsigned short height;

    // BMP FILE HEADER constants
    static constexpr uint8_t BFTYPE = 2;
    static constexpr uint8_t BFSIZE = 4;
    static constexpr uint8_t BFRESERVED1 = 2;
    static constexpr uint8_t BFRESERVED2 = 2;
    static constexpr uint8_t BFOFFBITS = 4;

    // BMP INFO HEADER constants
    static constexpr uint8_t BISIZE = 4;
    static constexpr uint8_t BIWIDTH = 4;
    static constexpr uint8_t BIHEIGHT = 4;
    static constexpr uint8_t BIPLANES = 2;
    static constexpr uint8_t BIBITCOUNT = 2;
    static constexpr uint8_t BICOMPRESSION = 4;
    static constexpr uint8_t BISIZEIMAGE = 4;
    static constexpr uint8_t BIXPELSPERMETER = 4;
    static constexpr uint8_t BIYPELSPERMETER = 4;
    static constexpr uint8_t BICLRUSED = 4;
    static constexpr uint8_t BICLRIMPORTANT = 4;

    // BMP PIXEL CONSTANTS
    static constexpr uint8_t RGBBLUE = 1;
    static constexpr uint8_t RGBGREEN = 1;
    static constexpr uint8_t RGBRED = 1;
    static constexpr uint8_t RGBRESERVED = 1;
    static constexpr uint8_t HEADERSIZE = 54;

    void write(ofstream& out, const uint8_t& n, const uint32_t& val) noexcept;

public:
    BMPGenerator();

    void setWidth(const uint16_t& width) noexcept;

    void setHeight(const uint16_t& height) noexcept;

    void generate(const void* data, const string& fileName) noexcept;
};

#endif // !BMP_GENERATOR_HPP
