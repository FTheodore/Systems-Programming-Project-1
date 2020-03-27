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

int insertAvlListToHeap(listNode * head, heapNode ** heapRoot, char type) {
    // update counters of heap based on all entries of an avl node
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

int buildHeap(avlNode * avlRoot, heapNode ** heapRoot, char type, bool datesGiven, date start, date end) {
    // build a new heap from the elements of a avl tree
    if(avlRoot != NULL) {
        int retVal;

        if(!datesGiven || (cmpDates(&start, &avlRoot->nodeDate) <= 0  && cmpDates(&avlRoot->nodeDate, &end) <= 0)) {
            retVal = insertAvlListToHeap(avlRoot->listHead, heapRoot, type);
            if(retVal)
                return -1;
        }

        retVal = buildHeap(avlRoot->lChild, heapRoot, type, datesGiven, start, end);
        if(retVal)
            return -1;

        retVal = buildHeap(avlRoot->rChild, heapRoot, type, datesGiven, start, end);
        if(retVal)
            return -1;

        return 0;
    }
    else
        return 0;
}

void getHeapParentNode(heapNode * root,heapNode * node, bool * isLeft, heapNode ** retAddress) {
    // find the parent of a node
    if(root == NULL)
        return;
    else if(root->lChild == node) {
        *isLeft = true;
        *retAddress = root;
        return;
    }
    else if(root->rChild == node) {
        *isLeft = false;
        *retAddress = root;
        return;
    }

    getHeapParentNode(root->lChild,node,isLeft,retAddress);
    getHeapParentNode(root->rChild,node,isLeft,retAddress);
}

void rightChildSwap(heapNode ** root, heapNode * node) {
    heapNode * rNode;
    rNode = node->rChild;

    if(*root == node)
        *root = rNode;
    else { // find the parent of the node
        bool isLeftChild;
        heapNode * parentNode;
        getHeapParentNode(*root,node,&isLeftChild,&parentNode);

        // update the child of parent
        if(isLeftChild)
            parentNode->lChild = rNode;
        else
            parentNode->rChild = rNode;
    }

    //swap right subtrees
    node->rChild = rNode->rChild;
    rNode->rChild = node;

    //swap left subtrees
    heapNode * tmp;
    tmp = node->lChild;
    node->lChild = rNode->lChild;
    rNode->lChild = tmp;
}

void leftChildSwap(heapNode ** root, heapNode * node) {
    heapNode * lNode;
    lNode = node->lChild;

    if(*root == node)
        *root = lNode;
    else { // find the parent of the node
        bool isLeftChild;
        heapNode * parentNode;
        getHeapParentNode(*root,node,&isLeftChild,&parentNode);

        // update the child of parent
        if(isLeftChild)
            parentNode->lChild = lNode;
        else
            parentNode->rChild = lNode;
    }

    //swap left subtrees
    node->lChild = lNode->lChild;
    lNode->lChild = node;

    //swap right subtrees
    heapNode * tmp;
    tmp = node->rChild;
    node->rChild = lNode->rChild;
    lNode->rChild = tmp;
}

void heapify(heapNode ** root, heapNode * node) { // fix max heap in a subtree
    int maxCount = node->count;

    if(node->lChild != NULL && node->lChild->count > maxCount)
        maxCount = node->lChild->count;

    if(node->rChild != NULL && node->rChild->count > maxCount)
        maxCount = node->rChild->count;

    if(maxCount == node->count) // max heap property not violated
        return;
    else if(node->lChild != NULL && node->lChild->count == maxCount) // left child bigger than root
        leftChildSwap(root,node);
    else // right child bigger
        rightChildSwap(root,node);

    heapify(root,node); // check if subtree max heap property is violated
}

void buildMaxHeap(heapNode * node, heapNode ** root) {
    if(node != NULL) {
        buildMaxHeap(node->lChild, root);
        buildMaxHeap(node->rChild, root);
        heapify(root, node);
    }
}

void swapRoot(heapNode * node, heapNode ** root) { // reach end of heap and make that node the new root
    if(node->rChild != NULL)
        swapRoot(node->rChild, root);
    else if(node->lChild != NULL)
        swapRoot(node->lChild, root);
    else { // reached end
        bool isLeftChild;
        heapNode * parentNode;
        getHeapParentNode(*root, node, &isLeftChild, &parentNode);

        if(isLeftChild)
            parentNode->lChild = NULL;
        else
            parentNode->rChild = NULL;

        node->lChild = (*root)->lChild;
        node->rChild = (*root)->rChild;
        *root = node;
    }

}

void freeHeap(heapNode ** root) {
    if(*root != NULL) {
        freeHeap(&(*root)->lChild);
        freeHeap(&(*root)->rChild);

        free((*root)->string);
        free(*root);
    }
}

void outputDupCounts(heapNode * heapRoot, int val) {
    if(heapRoot != NULL) {
        if(val == heapRoot->count)
            printf("%s %d\n", heapRoot->string, heapRoot->count);
        else // this is a max heap so children will definitely have a smaller count
            return;

        outputDupCounts(heapRoot->lChild, val);
        outputDupCounts(heapRoot->rChild, val);
    }
}

int getTopValues(int numOfValues, avlNode * avlRoot, char type, bool datesGiven, date start, date end) {
    heapNode * heapRoot = NULL;
    //first build the heap
    int retVal;
    retVal = buildHeap(avlRoot, &heapRoot, type, datesGiven, start, end);
    if(retVal)
        return -1;

    if(heapRoot == NULL) // no entries found
        return 0;

    buildMaxHeap(heapRoot, &heapRoot); // sort it

    for (int i = 0; i < numOfValues; ++i) {
        heapNode * nodeOut = heapRoot; // get the node with the highest value

        printf("%s %d\n", nodeOut->string, nodeOut->count);

        if(nodeOut->lChild == NULL && nodeOut->rChild == NULL) // last node
            break;


        swapRoot(heapRoot, &heapRoot); // replace root with leaf node
        heapify(&heapRoot, heapRoot); // fix max heap

        free(nodeOut->string);
        free(nodeOut);

        if(i == numOfValues - 1 && heapRoot->count == nodeOut->count)// output every entry with same count as the k-th
            outputDupCounts(heapRoot, heapRoot->count);

    }

    freeHeap(&heapRoot);
    return 0;
}