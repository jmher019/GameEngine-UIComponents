#include <BMPGenerator.hpp>

BMPGenerator::BMPGenerator() {}

void BMPGenerator::write(ofstream& out, const uint8_t& n, const uint32_t& val) noexcept {
    unsigned char c1, c2, c3, c4;
    switch (n) {
    case 1:
        c1 = c1 = (val & 0x000000ff);
        out << c1;
        break;
    case 2:
        c1 = (val & 0x0000ff00) >> 8;
        c2 = (val & 0x000000ff);
        out << c2 << c1;
        break;
    case 4:
        c1 = (val & 0xff000000) >> 24;
        c2 = (val & 0x00ff0000) >> 16;
        c3 = (val & 0x0000ff00) >> 8;
        c4 = (val & 0x000000ff);
        out << c4 << c3 << c2 << c1;
        break;
    }
}

void BMPGenerator::setWidth(const uint16_t& width) noexcept {
    this->width = width;
}

void BMPGenerator::setHeight(const uint16_t& height) noexcept {
    this->height = height;
}

void BMPGenerator::generate(const void* data, const string& fileName) noexcept {
    ofstream out(fileName.c_str(), std::ofstream::out | std::ofstream::binary);

    uint16_t size = width * height;

    // Here is the basic format for bmp files
    // 2 bytes for BFTYPE which is always set to 19778
    // 4 bytes for BFSIZE which is set to the total file size
    // 2 bytes for BFRESERVED1 which is always set to 0
    // 2 bytes for BFRESERVED2 which is always set to 0
    // 4 bytes for BFOFFBITS which is set to the sum of bytes of both headers
    //
    // 4 bytes for BISIZE size of bit map info header which is always set to 40
    // 4 bytes for BIWIDTH which is set to the image width in pixels
    // 4 bytes for BIHEIGHT which is set to the image height in pixels
    // 2 bytes for BIPLANES which is set to the number of color planes being used
    // 2 bytes for BIBITCOUNT which is the number of bits per pixel (24 for RGB)
    // 4 bytes for BICOMPRESSION which is the type of compression (0 for none)
    // 4 bytes for BIXPELSPERMETER which is the horizontal pixels per meter
    // 4 bytes for BIYPELSPERMETER which is the vertical pixels per meter
    // 4 bytes for BICLRUSED which is the number of colors used (if set to 0 then it is calculated from BIBITCOUNT)
    // 4 bytes for BICLRIMPORTANT which is the number of color that are important (if set to 0 then all are important)

    uint16_t remainder = width % 4;
    uint32_t fileSize = size * 3 + remainder * height + HEADERSIZE;

    write(out, BFTYPE, 19778);
    write(out, BFSIZE, fileSize);
    write(out, BFRESERVED1, 0);
    write(out, BFRESERVED2, 0);
    write(out, BFOFFBITS, HEADERSIZE);
    write(out, BISIZE, 40);
    write(out, BIWIDTH, width);
    write(out, BIHEIGHT, height);
    write(out, BIPLANES, 1);
    write(out, BIBITCOUNT, 24);
    write(out, BICOMPRESSION, 0);
    write(out, BISIZEIMAGE, size * 3);
    write(out, BIXPELSPERMETER, 0);
    write(out, BIYPELSPERMETER, 0);
    write(out, BICLRUSED, 0);
    write(out, BICLRIMPORTANT, 0);

    // The way to write the pixel data for RGB is to write
    // BGR. Also begin writing from the bottomost row and work
    // your way up. If the number of bytes written per row is not
    // divisible by 4 then pad it with bytes set to 0 until it is
    const uint8_t* pixels = (uint8_t*)data;
    for (uint16_t y = height - 1; y < height; y--) {
        size_t index = width * y;
        for (uint16_t x = 0; x < width; x++, index++) {
            uint8_t val = pixels[index];
            write(out, RGBBLUE, val);
            write(out, RGBGREEN, val);
            write(out, RGBRED, val);
        }

        if (remainder == 1) {
            write(out, RGBRESERVED, 0);
        }
        else if (remainder == 2) {
            write(out, RGBRESERVED, 0);
            write(out, RGBRESERVED, 0);
        }
        else if (remainder == 3) {
            write(out, RGBRESERVED, 0);
            write(out, RGBRESERVED, 0);
            write(out, RGBRESERVED, 0);
        }
    }

    // close the file
    out.close();
}