#include "lodepng.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int i, j;

//
char* load_png_file  (const char *filename, int *width, int *height) {
	unsigned char *image = NULL;
	int error = lodepng_decode32_file(&image, width, height, filename);
	if (error) {
		printf("error %u: %s\n", error, lodepng_error_text(error));
		return NULL;
	}

	return (image);
}

struct pixel { 
	char R;
	char G;
	char B; 
	char alpha;
};

int main() {
	int w = 0, h = 0;
	int k = 0;

	char *filename = "skull.png";
	char *picture = load_png_file(filename, &w, &h);

	if (picture == NULL) {
		printf("I can't read the picture %s. Error.\n", filename);
		return -1;
	}

	for (i = 0; i < h * w * 4; i += 4) {
		//This is an example how to operate with pixel 
		//encoded as RGBa / 4 bytes
		struct pixel P;
		P.R = picture[i+0];
		P.G = picture[i+1];
		P.B = picture[i+2];
		P.alpha = picture[i+3];

        char gray = (P.R + P.G + P.B) / 3;
        P.R = gray;
        P.G = gray;
        P.B = gray;

        picture[i+0] = P.R;
        picture[i+1] = P.G;
        picture[i+2] = P.B;
        picture[i+3] = P.alpha;
		k++;
	}

    char *output_filename = "output.png";
    unsigned error = lodepng_encode32_file(output_filename, picture, w, h);

    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return -1;
    }

	free(picture);


	return 0;
}
