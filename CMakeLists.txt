cmake_minimum_required(VERSION 3.26)
project(FKI_MSU_2nd_semester_2023_2024_Object_Detection C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(FKI_MSU_2nd_semester_2023_2024_Object_Detection
        image.c
        lodepng.c
        lodepng.h
        gayssa.c
        coloring.c)
