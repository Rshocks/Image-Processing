#ifndef IMAGE_H
#define IMAGE_H

#include <cstdio>
#include <cstdint>
#include <cstring>

#define STEG_HEADER_SIZE sizeof(uint32_t) * 8

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

    Image& diffmap(Image& img);
    Image& diffmap_scale(Image& img, uint8_t scale = 0);

    Image& greyscale_avg();
    Image& greyscale_lum();

    Image& colorMask(float r, float g, float b);

    Image& encodeMessage(const char* message);
    Image& decodeMessage(char* buffer, size_t* max_size);

    Image& flipX();
    Image& flipY();
    Image& oneChannel();

};

#endif