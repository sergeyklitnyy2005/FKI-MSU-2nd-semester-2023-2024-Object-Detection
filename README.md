# FKI-MSU-2nd-semester-2023-2024-Object-Detection

# Описание функций в файле `filter.c`:

1. **createFilter(double gKernel[][5])**: функция создает Гауссовский фильтр. Она принимает двумерный массив `gKernel` в качестве аргумента и заполняет его значениями Гауссовского ядра.

2. **applyFilter(unsigned char *image, int width, int height)**: функция применяет Гауссовский фильтр к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает Гауссовский фильтр и применяет его к изображению, обновляя каждый пиксель.

3. **applyCannyFilter(unsigned char *image, int width, int height)**: функция применяет фильтр Кэнни к изображению. Она принимает указатель на изображение и его ширину и высоту в качестве аргументов. Функция создает градиентное изображение и применяет пороговое значение для определения границ.

4. **perona_malik(unsigned char *image, int width, int height, float dt, float K, int N)**: функция применяет фильтр Перона-Малика к изображению. Она принимает указатель на изображение, его ширину и высоту, а также параметры `dt`, `K` и `N` в качестве аргументов. Функция выполняет диффузию Перона-Малика, которая помогает уменьшить шум на изображении, сохраняя при этом границы объектов.
