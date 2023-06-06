#include "ImageHead.h"

int main(int argc, char** argv) {
    /*Image img("test.png");

    // read write code
    img.write("test2.jpg");
    Image copy = img;

    for(int i = 0; i < copy.w*copy.channels; i++){
        copy.data[i] = 255;
    }
    copy.write("test3.jpg");
    Image blank(100, 100, 3);
    blank.write("test4.jpg");*/

    // greyscale code
    Image img("test.png");
    Image gray_avg = img;
    gray_avg.greyscale_avg();
    gray_avg.write("gray_avg.bmp");

    Image gray_lum = img;
    gray_lum.greyscale_lum();
    gray_lum.write("gray_lum.tga");
    
    return 0;
}