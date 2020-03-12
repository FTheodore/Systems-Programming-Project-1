cmake_minimum_required(VERSION 3.15)
project(SysPro_1 C)

set(CMAKE_C_STANDARD 99)

add_executable(SysPro_1 main.c structs.h miscFunctions.c miscFunctions.h patientList.c patientList.h avlTree.c avlTree.h)