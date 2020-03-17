//
// Created by theo on 14/3/20.
//

#ifndef SYSPRO_1_BINARYHEAP_H
#define SYSPRO_1_BINARYHEAP_H

#include <stdbool.h>
#include "miscFunctions.h"

heapNode * initHeapNode(char * string);
void findUpdateHeapNode(heapNode ** currentNode, char * newString, bool * foundNode);
int insertHeapNode(heapNode ** currentNode, char * newString, heapNode ** heapRoot);
int insertAvlListToHeap(listNode * head, heapNode ** heapRoot, char type);
int buildHeap(avlNode * avlRoot, heapNode ** heapRoot, char type, bool datesGiven, date start, date end);
void rightChildSwap(heapNode ** root, heapNode * node);
void leftChildSwap(heapNode ** root, heapNode * node);
void heapify(heapNode ** root, heapNode * node);
void buildMaxHeap(heapNode * node, heapNode ** root);
void swapRoot(heapNode * node, heapNode ** root);
void freeHeap(heapNode ** root);
int getTopValues(int numOfValues, avlNode * avlRoot, char type, bool datesGiven, date start, date end);

#endif //SYSPRO_1_BINARYHEAP_H
