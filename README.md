# FKI-MSU-2nd-semester-2023-2024-Object-Detection

# Описание функций в файле `filter.c`:

1. ``createFilter(double gKernel[][5])``: функция создает Гауссовский фильтр. Она принимает двумерный массив `gKernel` в качестве аргумента и заполняет его значениями Гауссовского ядра.

2. ``applyFilter(unsigned char *image, int width, int height)``: функция применяет Гауссовский фильтр к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает Гауссовский фильтр и применяет его к изображению, обновляя каждый пиксель.

3. ``applyCannyFilter(unsigned char *image, int width, int height)``: функция применяет фильтр Кэнни к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает градиентное изображение и применяет пороговое значение для определения границ.

4. ``perona_malik(unsigned char *image, int width, int height, float dt, float K, int N)``: функция применяет фильтр Перона-Малика к изображению. Она принимает указатель на изображение, его ширину и высоту, а также параметры `dt`, `K` и `N` в качестве аргументов. Функция выполняет диффузию Перона-Малика, которая помогает уменьшить шум на изображении, сохраняя при этом границы объектов.

5. ``applySobelFilter(unsigned char *image, int width, int height)``: функция применяет фильтр Собеля к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает градиентное изображение с использованием оператора Собеля и обновляет каждый пиксель изображения.

6. ``applyPrivetFilter(unsigned char *image, int width, int height)``: функция применяет Привет-фильтр к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает градиентное изображение с использованием Привет-оператора и обновляет каждый пиксель изображения.

7. ``applySharaFilter(unsigned char *image, int width, int height)``: функция применяет фильтр Щарра к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает градиентное изображение с использованием Щарра-оператора и обновляет каждый пиксель изображения.


# Описание функций в файле `coloring.c`:

1. ``Node* find(Node* x)``: функция реализует операцию поиска в структуре данных "система непересекающихся множеств". Она возвращает представителя множества, которому принадлежит узел `x`.

2. ``void union_set(Node* x, Node* y, double epsilon)``: функция объединяет два множества, которым принадлежат узлы `x` и `y`, если разница в цвете между `x` и `y` меньше заданного порога `epsilon`.

3. ``Node* create_graph(unsigned char *image, int *width, int *height)``: функция создает граф, где каждый узел представляет пиксель изображения. Узлы связаны с их соседями слева, справа, сверху и снизу.

4. ``void find_components(Node* nodes, int width, int height, double epsilon)``: функция проходит по всем узлам графа и объединяет соседние узлы, если разница в цвете между ними меньше `epsilon`.

5. ``void free_graph(Node* nodes)``: функция освобождает память, выделенную под граф.

6. ``void color_components_and_count(Node* nodes, unsigned char *image, int width, int height)``: функция окрашивает каждый компонент (связный подграф) в случайный цвет и подсчитывает количество компонентов.

7. ``void main_color_border(unsigned char* image, int w, int h, int epsilon)``: функция является основной функцией, которая вызывает все остальные функции для выполнения сегментации изображения.

8. ``void floodFill(unsigned char* image, int x, int y, int newColor1, int newColor2, int newColor3, int oldColor, int width, int height)``: функция реализует алгоритм заливки области, который изменяет цвет заданного пикселя и всех соседних пикселей с тем же цветом.

9. ``void colorComponents_filling(unsigned char* image, int width, int height, int epsilon)``: функция применяет алгоритм заливки области к каждому пикселю изображения, цвет которого меньше `epsilon`.

10. ``void floodFillRecursive(unsigned char* image, int x, int y, int newColor1, int newColor2, int newColor3, int oldColor, int width, int height)``: Это рекурсивная версия алгоритма заливки области.

11. ``void colorComponents_fillingRecursive(unsigned char* image, int width, int height, int epsilon)``: функция применяет рекурсивный алгоритм заливки области к каждому пикселю изображения, цвет которого меньше `epsilon`.  
