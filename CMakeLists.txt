cmake_minimum_required(VERSION 3.15)
project(SysPro_1 C)

set(CMAKE_C_STANDARD 99)

add_executable(SysPro_1 main.c structs.h miscFunctions.c miscFunctions.h patientList.c patientList.h avlTree.c avlTree.h hashTable.c hashTable.h binaryHeap.c binaryHeap.h)
target_link_libraries(SysPro_1 m)
