#include "ImageHead.h"

int main(int argc, char** argv) {
    Image img("test.png");

    // read write code
    /*img.write("test2.jpg");
    Image copy = img;

    for(int i = 0; i < copy.w*copy.channels; i++){
        copy.data[i] = 255;
    }
    copy.write("test3.jpg");
    Image blank(100, 100, 3);
    blank.write("test4.jpg");*/

    // greyscale code
    /*Image gray_avg = img;
    gray_avg.greyscale_avg();
    gray_avg.write("gray_avg.png");

    Image gray_lum = img;
    gray_lum.greyscale_lum();
    gray_lum.write("gray_lum.png");*/

    //color mask code
    /*img.colorMask(0.0f, 1.0f, 0.0f);
    img.write("green_mask.png");*/

    // Encode message code
    img.encodeMessage("MSG");
    img.write("encoded.png");

    char buffer[256] = {0};
    size_t len = 0;
    img.decodeMessage(buffer, &len);

    printf("Decoded message length: %zu\n", len);

    // Print the decoded message
    for (size_t i = 0; i < len; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");
    
    return 0;
}