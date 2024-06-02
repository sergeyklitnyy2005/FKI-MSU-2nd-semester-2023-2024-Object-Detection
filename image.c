#include "lodepng.c"
#include "filter.c"
#include "coloring.c"
#include <stdio.h>
#include <stdlib.h>
#define PI 3.14159265358979323846


char* load_png_file(const char *filename, int *width, int *height) {
    unsigned char *image = NULL;
    int error = lodepng_decode32_file(&image, width, height, filename);
    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return NULL;
    }

    return (image);
}


int main() {
    int w = 0, h = 0;

    char *filename = "skull.png";
    char *picture = load_png_file(filename, &w, &h);

    if (picture == NULL) {
        printf("I can't read the picture %s. Error.\n", filename);
        return -1;
    }
    applySobelFilter(picture, w, h);
    unsigned error = lodepng_encode32_file("output2.png", picture, w, h);
    
    colorComponents_filling(picture, w, h, 20);
    
    char *output_filename = "output.png";
    unsigned error = lodepng_encode32_file(output_filename, picture, w, h);

    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return -1;
    }

    free(picture);

    return 0;
}

