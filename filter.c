#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

void createFilter(double gKernel[][5])
{
    double r, s = 5.00;

    for (int x = -2; x <= 2; x++) {
        for(int y = -2; y <= 2; y++) {
            r = sqrt(x*x + y*y);
            gKernel[x + 2][y + 2] = (exp(-(r*r)/s))/(PI * s);
        }
    }
}

void applyFilter(unsigned char *image, int width, int height) {
    double gKernel[5][5];
    createFilter(gKernel);

    unsigned char *result = malloc(width * height * 4 * sizeof(unsigned char));

    for (int y = 2; y < height - 2; y++) {
        for (int x = 2; x < width - 2; x++) {
            double red = 0.0, green = 0.0, blue = 0.0;
            for (int dy = -2; dy <= 2; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    int index = ((y+dy) * width + (x+dx)) * 4;
                    red += image[index] * gKernel[dy + 2][dx + 2];
                    green += image[index + 1] * gKernel[dy + 2][dx + 2];
                    blue += image[index + 2] * gKernel[dy + 2][dx + 2];
                }
            }
            int resultIndex = (y * width + x) * 4;
            result[resultIndex] = (unsigned char)round(red);
            result[resultIndex + 1] = (unsigned char)round(green);
            result[resultIndex + 2] = (unsigned char)round(blue);
            result[resultIndex + 3] = image[resultIndex + 3];
        }
    }

    for (int i = 0; i < width * height * 4; i++) {
        image[i] = result[i];
    }

    free(result);
}

void applyCannyFilter(unsigned char *image, int width, int height) {
    int gx[3][3] = {{3, 0, 3}, {10, 0, -10}, {3, 0, -3}};
    int gy[3][3] = {{3, 10, 3}, {0, 0, 0}, {-3, -10, -3}};


    unsigned char *result = malloc(width * height * 4 * sizeof(unsigned char));
    unsigned char *gradient = malloc(width * height * sizeof(unsigned char));
    int *direction = malloc(width * height * sizeof(int));

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
            gradient[(y * width + x)] = (unsigned char) magnitude;
            direction[(y * width + x)] = (int)(atan2(sumY, sumX) * 180 / PI);

        }
    }

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int dir = direction[y * width + x];
            dir = (dir + 180) % 180;

            if (!((dir > 67.5 && dir <= 112.5 && gradient[y * width + x] <= gradient[(y+1) * width + x] && gradient[y * width + x] <= gradient[(y-1) * width + x]) ||
                  (dir > 112.5 && dir <= 157.5 && gradient[y * width + x] <= gradient[(y+1) * width + (x-1)] && gradient[y * width + x] <= gradient[(y-1) * width + (x+1)]) ||
                  (dir > 22.5 && dir <= 67.5 && gradient[y * width + x] <= gradient[(y+1) * width + (x+1)] && gradient[y * width + x] <= gradient[(y-1) * width + (x-1)]) ||
                  ((dir >= 0 && dir <= 22.5 || dir > 157.5 && dir <= 180) && gradient[y * width + x] <= gradient[y * width + (x+1)] && gradient[y * width + x] <= gradient[y * width + (x-1)]))) {
                gradient[y * width + x] = 0;
            }
        }
    }

    int highThreshold = 75;
    int lowThreshold = 25;

    for (int i = 0; i < width * height; i++) {
        if (gradient[i] >= highThreshold) {
            gradient[i] = 255;
        } else if (gradient[i] < lowThreshold) {
            gradient[i] = 0;
        } else {
            gradient[i] = 75;
        }
    }


    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (gradient[y * width + x] != 75) {
                continue;
            }

            if ((gradient[(y-1) * width + (x-1)] == 255) || (gradient[(y-1) * width + x] == 255) || (gradient[(y-1) * width + (x+1)] == 255) ||
                (gradient[y * width + (x-1)] == 255) || (gradient[y * width + (x+1)] == 255) ||
                (gradient[(y+1) * width + (x-1)] == 255) || (gradient[(y+1) * width + x] == 255) || (gradient[(y+1) * width + (x+1)] == 255)) {
                gradient[y * width + x] = 255;
            } else {
                gradient[y * width + x] = 0;
            }
        }
    }
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int resultIndex = (y * width + x) * 4;
            image[resultIndex] = (unsigned char) gradient[y * width + x];
            image[resultIndex + 1] = (unsigned char) gradient[y * width + x];
            image[resultIndex + 2] = (unsigned char) gradient[y * width + x];
            image[resultIndex + 3] = image[resultIndex + 3];
        }
    }
}

void perona_malik(unsigned char *image, int width, int height, float dt, float K, int N) {
    float dx, dy, dxx, dyy, dxy, grad;
    float cN, cS, cW, cE;

    unsigned char *result = malloc(width * height * sizeof(unsigned char));

    for (int t = 0; t < N; t++) {
        for (int i = 1; i < height-1; i++) {
            for (int j = 1; j < width-1; j++) {
                dx = (image[4 * ((i+1)*width+j)] - image[ 4 * ((i-1)*width+j)]) / 2.0;
                dy = (image[4 *(i*width+(j+1))] - image[4 *(i*width+(j-1))]) / 2.0;
                dxx = image[4 *((i+1)*width+j)] - 2*image[4 *(i*width+j)] + image[4 *((i-1)*width+j)];
                dyy = image[4 *(i*width+(j+1))] - 2*image[4 *(i*width+j)] + image[4 *(i*width+(j-1))];
                dxy = (image[4 *((i+1)*width+(j+1))] - image[4 *((i+1)*width+(j-1))] - image[4 *((i-1)*width+(j+1))] + image[4 *((i-1)*width+(j-1))]) / 4.0;
                grad = dx*dx + dy*dy;

                cN = exp(-(dx*dx + dxy*dxy) / (K*K));
                cS = exp(-(dx*dx + dxy*dxy) / (K*K));
                cW = exp(-(dy*dy + dxy*dxy) / (K*K));
                cE = exp(-(dy*dy + dxy*dxy) / (K*K));

                result[(i*width+j)] = image[4 *(i*width+j)] + dt * (cN*dxx + cS*dxx + cW*dyy + cE*dyy);
            }
        }
        for (int i = 0; i < width * height; i++) {
            int resultIndex = i * 4;
            image[resultIndex] = (unsigned char) result[i];
            image[resultIndex + 1] = (unsigned char) result[i];
            image[resultIndex + 2] = (unsigned char) result[i];
            image[resultIndex + 3] = image[resultIndex + 3];

        }
    }

    free(result);
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

void applyPrivetFilter(unsigned char *image, int width, int height) {
    int gx[3][3] = {{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}};
    int gy[3][3] = {{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}};


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

void applySharaFilter(unsigned char *image, int width, int height) {
    int gx[3][3] = {{3, 0, -3}, {10, 0, -10}, {3, 0, -3}};
    int gy[3][3] = {{3, 10, 3}, {0, 0, 0}, {-3, -10, -3}};


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

