#include "lodepng.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct pixel {
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char alpha;
};

char* load_png_file(const char *filename, int *width, int *height) {
    unsigned char *image = NULL;
    int error = lodepng_decode32_file(&image, width, height, filename);
    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return NULL;
    }

    return (image);
}

void applySobelFilter(unsigned char *image, int width, int height) {
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

    unsigned char *result = malloc(width * height * 4 * sizeof(unsigned char));

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumX = 0, sumY = 0;
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int index = ((y+dy) * width + (x+dx)) * 4;
                    int gray = (image[index] + image[index + 1] + image[index + 2]) / 3;
                    sumX += gx[dy + 1][dx + 1] * gray;
                    sumY += gy[dy + 1][dx + 1] * gray;
                }
            }
            int magnitude = sqrt(sumX * sumX + sumY * sumY);
            if (magnitude > 255) magnitude = 255;
            if (magnitude < 0) magnitude = 0;

            int resultIndex = (y * width + x) * 4;
            result[resultIndex] = (unsigned char)magnitude;
            result[resultIndex + 1] = (unsigned char)magnitude;
            result[resultIndex + 2] = (unsigned char)magnitude;
            result[resultIndex + 3] = image[resultIndex + 3];
        }
    }

    for (int i = 0; i < width * height * 4; i++) {
        image[i] = result[i];
    }

    free(result);
}

int main() {
    int w = 0, h = 0;

    char *filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\skull.png";
    char *picture = load_png_file(filename, &w, &h);

    if (picture == NULL) {
        printf("I can't read the picture %s. Error.\n", filename);
        return -1;
    }

    applySobelFilter(picture, w, h);

    char *output_filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\output.png";
    unsigned error = lodepng_encode32_file(output_filename, picture, w, h);

    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return -1;
    }

    free(picture);

    return 0;
}

