#include <stdlib.h>
#include <math.h>
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

Node* create_graph(unsigned char *image, int *width, int *height) {

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

void color_components_and_count(Node* nodes, unsigned char *image, int width, int height) {
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

//    char *output_filename = "C:\\Users\\ksv\\Documents\\GitHub\\FKI-MSU-2nd-semester-2023-2024-Object-Detection\\output2.png";
//    lodepng_encode32_file(output_filename, output_image, width, height);

//    printf("Total components: %d\n", total_components);
//    for (int i = 0; i < width * height; i++) {
//        if (component_sizes[i] > 0) {
//            printf("Component %d size: %d\n", i, component_sizes[i]);
//        }
//    }

    for (int i = 0; i < width * height * 4; i++) {
        image[i] = output_image[i];
    }

    free(output_image);
    free(component_sizes);
}

void main_color_border(unsigned char* image, int w, int h, int epsilon) {
    Node* nodes = create_graph(image, &w, &h);

    find_components(nodes, w, h, epsilon);
    color_components_and_count(nodes, image, w, h);

    free_graph(nodes);
}


void floodFill(unsigned char* image, int x, int y, int newColor1, int newColor2, int newColor3, int oldColor, int width, int height) {
    typedef struct {int x, y;} Point;
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};

    Point* stack = malloc(width * height * 4 * sizeof(Point));
    long top = 0;

    stack[top++] = (Point){x, y};

    while(top > 0) {
        Point p = stack[--top];

        if(p.x < 0 || p.x >= width || p.y < 0 || p.y >= height)
            continue;

        int resultIndex = (p.y * width + p.x) * 4;
        if(image[resultIndex] > oldColor)
            continue;

        image[resultIndex] = newColor1;
        image[resultIndex + 1] = newColor2;
        image[resultIndex + 2] = newColor3;


        for(int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];
            int nIndex = (ny * width + nx) * 4;
            if(nx > 0 && nx < width && ny > 0 && ny < height && image[nIndex] <= oldColor) {
                stack[top++] = (Point){nx, ny};
            }
        }
    }
    free(stack);
}

void colorComponents_filling(unsigned char* image, int width, int height, int epsilon) {
    int color1 = epsilon * 2, color2 = epsilon * 2, color3 = epsilon * 2;
    for(int y = 1; y < height - 1; y++) {
        for(int x = 1; x < width - 1; x++) {
            if(image[4 * (y * width + x)] < epsilon) {
                floodFill(image, x, y, color1, color2, color3, epsilon, width, height);
                color1 = rand() % (255 - epsilon * 2) + epsilon * 2;
                color2 = rand() % (255 - epsilon * 2) + epsilon * 2;
                color3 = rand() % (255 - epsilon * 2) + epsilon * 2;
            }

        }
    }
}

void floodFillRecursive(unsigned char* image, int x, int y, int newColor1, int newColor2, int newColor3, int oldColor, int width, int height) {
    if(x < 0 || x >= width || y < 0 || y >= height)
        return;

    int resultIndex = (y * width + x) * 4;
    if(image[resultIndex] > oldColor)
        return;

    image[resultIndex] = newColor1;
    image[resultIndex + 1] = newColor2;
    image[resultIndex + 2] = newColor3;

    floodFillRecursive(image, x+1, y, newColor1, newColor2, newColor3, oldColor, width, height);
    floodFillRecursive(image, x-1, y, newColor1, newColor2, newColor3, oldColor, width, height);
    floodFillRecursive(image, x, y+1, newColor1, newColor2, newColor3, oldColor, width, height);
    floodFillRecursive(image, x, y-1, newColor1, newColor2, newColor3, oldColor, width, height);
}


void colorComponents_fillingRecursive(unsigned char* image, int width, int height, int epsilon) {
    int color1, color2, color3;
    for(int y = 1; y < height - 1; y++) {
        for(int x = 1; x < width - 1; x++) {
            if(image[4 * (y * width + x)] < epsilon) {
                color1 = rand() % (255 - epsilon * 2) + epsilon * 2;
                color2 = rand() % (255 - epsilon * 2) + epsilon * 2;
                color3 = rand() % (255 - epsilon * 2) + epsilon * 2;
                floodFillRecursive(image, x, y, color1, color2, color3, epsilon, width, height);

            }
        }
    }
}