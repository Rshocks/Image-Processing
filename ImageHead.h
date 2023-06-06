#ifndef IMAGE_H
#define IMAGE_H

#include <cstdio>
#include <cstdint>
#include <cstring>

enum ImageType {
    PNG, JPG, BMP, TGA
};

struct Image {
    uint8_t* data = nullptr;
    size_t size = 0;
    int w;
    int h;
    int channels;

    Image(const char* filename);
    Image(int w, int h, int channels);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType(const char* filename);

    Image& greyscale_avg();
    Image& greyscale_lum();
};

#endif