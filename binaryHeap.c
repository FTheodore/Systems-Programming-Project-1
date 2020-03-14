//
// Created by theo on 14/3/20.
//

#include "binaryHeap.h"

heapNode * initHeapNode(char * string) {
    heapNode * newNode = malloc(sizeof(heapNode));
    if(memErrCheck(newNode))
        return NULL;

    newNode->lChild = NULL;
    newNode->rChild = NULL;
    newNode->string = strdup(string); // this string will be either a country or a disease
    if(memErrCheck(newNode->string))
        return NULL;

    newNode->count = 1;
    newNode->height = 1;

    return newNode;
}

int insertHeapNode(heapNode ** currentNode, char * newString, heapNode ** heapRoot) {
    if(*currentNode == NULL) { // reached end of heap
        *currentNode = initHeapNode(newString);
        if(*currentNode == NULL)
            return -1;
        updateHeapHeight(*heapRoot);
        return 0;
    }
    else if(getHeapSubtreeHeightDiff(*currentNode) > 0) // left subtree is bigger so insert to right subtree
        return insertHeapNode(&(*currentNode)->rChild, newString, heapRoot);
    else
        return insertHeapNode(&(*currentNode)->lChild, newString, heapRoot);

}

void findUpdateHeapNode(heapNode ** currentNode, char * newString, bool * foundNode) {
    // this function will check if a node with same string already exists and update it's counter
    if(*currentNode == NULL) { // reached end of heap
        return;
    }
    else if(strcmp((*currentNode)->string,newString) == 0) { // found node with same string
        (*currentNode)->count += 1;
        *foundNode = true;
        return;
    }

    findUpdateHeapNode(&(*currentNode)->lChild, newString, foundNode);
    if(*foundNode)
        return;

    findUpdateHeapNode(&(*currentNode)->rChild, newString, foundNode);
}

void updateHeapHeight(heapNode * node) {
    if(node != NULL) {
        updateHeapHeight(node->lChild); // first update the height of the subtrees
        updateHeapHeight(node->rChild);

        if(node->lChild == NULL && node->rChild == NULL) {
            node->height = 1;
        }
        else if(node->lChild == NULL) {
            node->height = node->rChild->height + 1;
        }
        else if(node->rChild == NULL) {
            node->height = node->lChild->height + 1;
        }
        else { // get max subtree height
            if(node->rChild->height >= node->lChild->height)
                node->height = node->rChild->height + 1;
            else
                node->height = node->lChild->height + 1;
        }
    }
}

int getHeapSubtreeHeightDiff(heapNode * node) {
    // this function will return the difference of the subtree heights

    if(node->lChild == NULL && node->rChild == NULL)
        return 0;

    if(node->rChild == NULL)
        return node->lChild->height;

    if(node->lChild == NULL)
        return -node->rChild->height;

    return node->lChild->height - node->rChild->height;
}

int insertAvlListToHeap(listNode * head, heapNode ** heapRoot, char type) {
    if(head != NULL) {
        bool foundNode = false;
        patientRecord * ptr = ((listNode *)head->dataPointer)->dataPointer;
        if(type == 'c') {
            findUpdateHeapNode(heapRoot, ptr->country, &foundNode);
            if(!foundNode) {
                int retVal = insertHeapNode(heapRoot, ptr->country, heapRoot);
                if(retVal)
                    return -1;
            }
        }
        else if(type == 'd') {
            findUpdateHeapNode(heapRoot, ptr->diseaseId, &foundNode);
            if(!foundNode) {
                int retVal = insertHeapNode(heapRoot, ptr->diseaseId, heapRoot);
                if(retVal)
                    return -1;
            }
        }

        return insertAvlListToHeap(head->next, heapRoot, type);
    }
    else
        return 0;
}

int buildHeap(avlNode * avlRoot, heapNode ** heapRoot, char type) {
    // build a new heap from the elements of a avl tree
    if(avlRoot != NULL) {
        int retVal;
        retVal = insertAvlListToHeap(avlRoot->listHead, heapRoot, type);
        if(retVal)
            return -1;

        retVal = buildHeap(avlRoot->lChild, heapRoot, type);
        if(retVal)
            return -1;

        retVal = buildHeap(avlRoot->rChild, heapRoot, type);
        if(retVal)
            return -1;

        return 0;
    }
    else
        return 0;
}