#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define BYTE_BOUND(value) value < 0 ? 0 : (value > 255 ? 255 : value)
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "stb_image.h"
#include "stb_image_write.h"
#include "ImageHead.h"

Image::Image(const char* filename) {
    if (read(filename)) {
        printf("Read %s\n", filename);
        size = w * h * channels;
    } else {
        printf("Failed to read %s\n", filename);
    }
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels) {
    size = w * h * channels;
    data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels) {
    memcpy(data, img.data, size);
}

Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename) {
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != nullptr;
}

bool Image::write(const char* filename) {
    ImageType type = getFileType(filename);
    int success;
    switch (type) {
        case PNG:
            success = stbi_write_png(filename, w, h, channels, data, w * channels);
            break;
        case BMP:
            success = stbi_write_bmp(filename, w, h, channels, data);
            break;
        case JPG:
            success = stbi_write_jpg(filename, w, h, channels, data, 100);
            break;
        case TGA:
            success = stbi_write_tga(filename, w, h, channels, data);
            break;
    }
    return success != 0;
}

ImageType Image::getFileType(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (ext != nullptr) {
        if (strcmp(ext, ".png") == 0) {
            return PNG;
        } else if (strcmp(ext, ".jpg") == 0) {
            return JPG;
        } else if (strcmp(ext, ".bmp") == 0) {
            return BMP;
        } else if (strcmp(ext, ".tga") == 0) {
            return TGA;
        }
    }
    return PNG;
}

Image& Image::greyscale_avg() {
    // r+g+b/3
    if (channels < 3) {
        printf("Not enough channels to convert to greyscale\n");
    } else {
        for (int i = 0; i < size; i += channels) {
            int gray = (data[i] + data[i + 1] + data[i + 2]) / 3;
            data[i] = gray;
            data[i + 1] = gray;
            data[i + 2] = gray;
        }
    }
    return *this;
}

Image& Image::greyscale_lum() {
    if (channels < 3) {
        printf("Not enough channels to convert to greyscale\n");
    } else {
        for (int i = 0; i < size; i += channels) {
            int gray = 0.2126 * data[i] + 0.7152 * data[i + 1] + 0.0722 * data[i + 2];
            data[i] = gray;
            data[i + 1] = gray;
            data[i + 2] = gray;
        }
    }
    return *this;
}

Image& Image::colorMask(float r, float g, float b){
    if(channels < 3){
        printf("Not enough channels to apply color mask\n");
    } else {
        for(int i = 0; i < size; i += channels){
            data[i] *= r;
            data[i + 1] *= g;
            data[i + 2] *= b;
        }
    }
    return *this;
}

Image& Image::encodeMessage(const char* message){
    uint8_t len = strlen(message) * 8;

    if(len + STEG_HEADER_SIZE > size){
        printf("Message too long to encode\n");
        return *this;
    }

    for(uint8_t i = 0; i < STEG_HEADER_SIZE; i++){
        printf("Length: %d\n", len >> (STEG_HEADER_SIZE - 1 - i) & 1UL);
        data[i] &= 0xFE;
        data[i] |= (len >> (STEG_HEADER_SIZE - 1 - i)) & 1UL;
    }

    for(uint32_t i = 0; i < len; i++){
        data[i+STEG_HEADER_SIZE] &= 0xFE;
        data[i+STEG_HEADER_SIZE] |= (message[i/8] >> ((len-1-i)%8)) & 1UL;
    }
    return *this;
}

Image& Image::decodeMessage(char* buffer, size_t* max_size){
    uint32_t len = 0;
    for(uint8_t i = 0; i < STEG_HEADER_SIZE; i++){
        len = (len << 1) | (data[i] & 1);
    }
    *max_size = len / 8;

    for(uint32_t i = 0; i < len; i++){
        buffer[i/8] = (buffer[i/8] << 1) | (data[i+STEG_HEADER_SIZE] & 1);
    }
    return *this;
}

Image& Image::diffmap(Image& img){
    int compare_width = fmin(w, img.w);
    int compare_height = fmin(h, img.h);
    int compare_channels = fmin(channels, img.channels);
    for(uint32_t i = 0; i < compare_height; i++){
        for(uint32_t j=0; j < compare_width; j++){
            for(uint8_t k = 0; k < compare_channels; k++){
                data[(i*w + j)*channels + k] = BYTE_BOUND(abs(data[(i*w + j)*channels + k] - img.data[(i*img.w + j)*img.channels + k]));
            }
        }
    }
    return *this;
}

Image& Image::flipX(){
    uint8_t temp[4];
    uint8_t* pixel1;
    uint8_t* pixel2;
    for(int y = 0;y < h; ++y){
        for(int x = 0;x < w/2; ++x){
            pixel1 = &data[(x + y * w) * channels];
            pixel2 = &data[((w - x - 1) + y * w) * channels];
            memcpy(temp, pixel1, channels);
            memcpy(pixel1, pixel2, channels);
            memcpy(pixel2, temp, channels);
        }
    }
    return *this;
}

Image& Image::flipY(){
    uint8_t temp[4];
    uint8_t* pixel1;
    uint8_t* pixel2;
    for(int x = 0;x < h; ++x){
        for(int y = 0;y < h/2; ++y){
            pixel1 = &data[(x + y * w) * channels];
            pixel2 = &data[(x + (h - 1 - y) * w) * channels];
            memcpy(temp, pixel1, channels);
            memcpy(pixel1, pixel2, channels);
            memcpy(pixel2, temp, channels);
        }
    }
    return *this;
}

Image& Image::oneChannel(){
    uint8_t temp[4];
    uint8_t* pixel1;
    uint8_t* pixel2;
    for(int x = 0;x < h; ++x){
        for(int y = 0;y < h/2; ++y){
            pixel1 = &data[(x + y * w) * channels];
            pixel2 = &data[(x + (h - 1 - y) * w) * channels];
            memcpy(temp, pixel1, 2);
            memcpy(pixel1, pixel2, 2);
            memcpy(pixel2, temp, 2);
        }
    }
    return *this;
}