//#include "lodepng.h"
//#include <math.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//#define PI 3.14159265358979323846
//
//char* load_png_file  (const char *filename, int *width, int *height) {
//    unsigned char *image = NULL;
//    int error = lodepng_decode32_file(&image, width, height, filename);
//    if (error) {
//        printf("error %u: %s\n", error, lodepng_error_text(error));
//        return NULL;
//    }
//
//    return (image);
//}
//
//struct pixel {
//    unsigned char R;
//    unsigned char G;
//    unsigned char B;
//    unsigned char alpha;
//};
//
//void createFilter(double gKernel[][5])
//{
//    double r, s = 5.0;
//
//    for (int x = -2; x <= 2; x++) {
//        for(int y = -2; y <= 2; y++) {
//            r = sqrt(x*x + y*y);
//            gKernel[x + 2][y + 2] = (exp(-(r*r)/s))/(PI * s);
//        }
//    }
//}
//
//void applyFilter(unsigned char *image, int width, int height) {
//    double gKernel[5][5];
//    createFilter(gKernel);
//
//    unsigned char *result = malloc(width * height * 4 * sizeof(unsigned char));
//
//    for (int y = 2; y < height - 2; y++) {
//        for (int x = 2; x < width - 2; x++) {
//            double red = 0.0, green = 0.0, blue = 0.0;
//            for (int dy = -2; dy <= 2; dy++) {
//                for (int dx = -2; dx <= 2; dx++) {
//                    int index = ((y+dy) * width + (x+dx)) * 4;
//                    red += image[index] * gKernel[dy + 2][dx + 2];
//                    green += image[index + 1] * gKernel[dy + 2][dx + 2];
//                    blue += image[index + 2] * gKernel[dy + 2][dx + 2];
//                }
//            }
//            int resultIndex = (y * width + x) * 4;
//            result[resultIndex] = (unsigned char)round(red);
//            result[resultIndex + 1] = (unsigned char)round(green);
//            result[resultIndex + 2] = (unsigned char)round(blue);
//            result[resultIndex + 3] = image[resultIndex + 3];
//        }
//    }
//
//    for (int i = 0; i < width * height * 4; i++) {
//        image[i] = result[i];
//    }
//
//    free(result);
//}
//
//int main() {
//    int w = 0, h = 0;
//    int k = 0;
//
//    char *filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\skull.png";
//    char *picture = load_png_file(filename, &w, &h);
//
//    if (picture == NULL) {
//        printf("I can't read the picture %s. Error.\n", filename);
//        return -1;
//    }
//
//    applyFilter(picture, w, h);
//
//
//
//	for (int i = 0; i < h * w * 4; i += 4) {
//		struct pixel P;
//		P.R = picture[i+0];
//		P.G = picture[i+1];
//		P.B = picture[i+2];
//		P.alpha = picture[i+3];
//
//        char gray = (P.R + P.G + P.B) / 3;
//        P.R = gray;
//        P.G = gray;
//        P.B = gray;
//
//        picture[i+0] = P.R;
//        picture[i+1] = P.G;
//        picture[i+2] = P.B;
//        picture[i+3] = P.alpha;
//		k++;
//	}
//
//    char *output_filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\output.png";
//    unsigned error = lodepng_encode32_file(output_filename, picture, w, h);
//
//    if (error) {
//        printf("error %u: %s\n", error, lodepng_error_text(error));
//        return -1;
//    }
//
//	free(picture);
//
//
//	return 0;
//}
