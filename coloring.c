#include "lodepng.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

typedef struct Node {
    unsigned char r, g, b, a;
    struct Node *up, *down, *left, *right;
    struct Node *parent;
    int rank;
} Node;

Node* find(Node* x) {
    if (x->parent != x) {
        x->parent = find(x->parent);
    }
    return x->parent;
}

void union_set(Node* x, Node* y, double epsilon) {
    if (x->r < 40 && y->r < 40) {
        return;
    }
    Node* px = find(x);
    Node* py = find(y);

    double color_difference = sqrt(pow(x->r - y->r, 2) + pow(x->g - y->g, 2) + pow(x->b - y->b, 2));
    if (px != py && color_difference < epsilon) {
        if (px->rank > py->rank) {
            py->parent = px;
        } else {
            px->parent = py;
            if (px->rank == py->rank) {
                py->rank++;
            }
        }
    }
}

Node* create_graph(const char *filename, int *width, int *height) {
    unsigned char *image = NULL;
    int error = lodepng_decode32_file(&image, width, height, filename);
    if (error) {
        printf("error %u: %s\n", error, lodepng_error_text(error));
        return NULL;
    }

    Node* nodes = malloc(*width * *height * sizeof(Node));
    if (!nodes) {
        free(image);
        return NULL;
    }

    for (unsigned y = 0; y < *height; ++y) {
        for (unsigned x = 0; x < *width; ++x) {
            Node* node = &nodes[y * *width + x];
            unsigned char* pixel = &image[(y * *width + x) * 4];
            node->r = pixel[0];
            node->g = pixel[1];
            node->b = pixel[2];
            node->a = pixel[3];
            node->up = y > 0 ? &nodes[(y - 1) * *width + x] : NULL;
            node->down = y < *height - 1 ? &nodes[(y + 1) * *width + x] : NULL;
            node->left = x > 0 ? &nodes[y * *width + (x - 1)] : NULL;
            node->right = x < *width - 1 ? &nodes[y * *width + (x + 1)] : NULL;
            node->parent = node;
            node->rank = 0;
        }
    }

    free(image);
    return nodes;
}

void find_components(Node* nodes, int width, int height, double epsilon) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Node* node = &nodes[y * width + x];
            if (node->up) {
                union_set(node, node->up, epsilon);
            }
            if (node->down) {
                union_set(node, node->down, epsilon);
            }
            if (node->left) {
                union_set(node, node->left, epsilon);
            }
            if (node->right) {
                union_set(node, node->right, epsilon);
            }
        }
    }
}

void free_graph(Node* nodes) {
    free(nodes);
}

void color_components_and_count(Node* nodes, int width, int height) {
    unsigned char* output_image = malloc(width * height * 4 * sizeof(unsigned char));
    int* component_sizes = calloc(width * height, sizeof(int));
    int total_components = 0;

    srand(time(NULL));
    for (int i = 0; i < width * height; i++) {
        Node* p = find(&nodes[i]);
        if (p == &nodes[i]) {
            if (component_sizes[i] < 3) {
                p->r = 0;
                p->g = 0;
                p->b = 0;
            } else {
                p->r = rand() % 256;
                p->g = rand() % 256;
                p->b = rand() % 256;
            }
            total_components++;
        }
        output_image[4 * i + 0] = p->r;
        output_image[4 * i + 1] = p->g;
        output_image[4 * i + 2] = p->b;
        output_image[4 * i + 3] = 255;
        component_sizes[p - nodes]++;
    }

    char *output_filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\output2.png";
    lodepng_encode32_file(output_filename, output_image, width, height);

    printf("Total components: %d\n", total_components);
//    for (int i = 0; i < width * height; i++) {
//        if (component_sizes[i] > 0) {
//            printf("Component %d size: %d\n", i, component_sizes[i]);
//        }
//    }

    free(output_image);
    free(component_sizes);
}

int main() {
    int width, height;
    char *filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\output.png";
    Node* nodes = create_graph(filename, &width, &height);
    if (!nodes) {
        return 1;
    }

    double epsilon = 50.0;
    find_components(nodes, width, height, epsilon);
    color_components_and_count(nodes, width, height);

    free_graph(nodes);
    return 0;
}
