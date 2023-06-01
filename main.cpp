#include "ImageHead.h"

int main(int argc, char** argv) {
    Image img("test.png");
    img.write("test2.jpg");
    Image copy = img;

    for(int i = 0; i < copy.w*copy.channels; i++){
        copy.data[i] = 255;
    }
    copy.write("test3.jpg");
    Image blank(100, 100, 3);
    blank.write("test4.jpg");
    
    return 0;
}