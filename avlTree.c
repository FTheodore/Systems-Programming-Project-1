//
// Created by theo on 12/3/20.
//

#include "avlTree.h"

avlNode * initAvlNode(date const * nodeDate) { // initialize a node for the balanced binary tree
    avlNode * newNode;

    newNode = malloc(sizeof(avlNode));
    if(memErrCheck(newNode))
        return NULL;

    newNode->lChild = NULL;
    newNode->rChild = NULL;
    newNode->listHead = NULL;
    newNode->height = 1;

    copyDate(&newNode->nodeDate, nodeDate);


    return newNode;
}

listNode * initListNodeOfAvl(listNode * patientRecord) {
    listNode * newNode;

    newNode = malloc(sizeof(listNode));
    if(memErrCheck(newNode))
        return NULL;

    newNode->next = NULL;
    newNode->dataPointer = patientRecord;

    return newNode;
}

avlNode * getParentNode(avlNode *root,avlNode *node, bool * isLeft) {
    if(root->lChild == node) {
        *isLeft = true;
        return root;
    }
    else if(root->rChild == node) {
        *isLeft = false;
        return root;
    }

    if(cmpDates(&root->nodeDate,&node->nodeDate) > 0)
        return getParentNode(root->lChild,node,isLeft);
    else
        return getParentNode(root->rChild,node,isLeft);
}

void rotateLeft(avlNode **root,avlNode *node) {
    avlNode * rNode;
    rNode = node->rChild; //right child of the node

    if(*root == node)
        *root = rNode;
    else { // find the parent of the node
        bool isLeftChild;
        avlNode * parentNode = getParentNode(*root,node,&isLeftChild);

        // update the child of parent
        if(isLeftChild)
            parentNode->lChild = rNode;
        else
            parentNode->rChild = rNode;
    }

    node->rChild = rNode->lChild;
    rNode->lChild = node;
}

void rotateRight(avlNode **root,avlNode *node) {
    avlNode * lNode;
    lNode = node->lChild; //left child of the node

    if(*root == node)
        *root = lNode;
    else { // find the parent of the node
        bool isLeftChild;
        avlNode * parentNode = getParentNode(*root,node,&isLeftChild);

        // update the child of parent
        if(isLeftChild)
            parentNode->lChild = lNode;
        else
            parentNode->rChild = lNode;
    }

    node->lChild = lNode->rChild;
    lNode->rChild = node;
}

void updateHeight(avlNode * node) {
    if(node != NULL) {
        updateHeight(node->lChild); // first update the height of the subtrees
        updateHeight(node->rChild);

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

int insertToAvlList(listNode ** listHead, listNode * patientRecord) {
    // insert a new pointer to a record, to the end of a list of an avl node
    if(*listHead == NULL) { // empty list check
        *listHead = initListNodeOfAvl(patientRecord);
        if(*listHead == NULL)
            return -1;
        return 0;
    }
    else if((*listHead)->next != NULL)
        return insertToAvlList(&(*listHead)->next,patientRecord);
    else { // reached end of list
        (*listHead)->next = initListNodeOfAvl(patientRecord);
        if((*listHead)->next == NULL)
            return -1;
        return 0;
    }
}

int insertToAvlTree(avlNode ** node, date * newDate, listNode * patientRecord, avlNode ** treeRoot) {
    // insert a new node to the avl tree and rebalance it if needed
    if(*node == NULL) { // reached end
        *node = initAvlNode(newDate);
        if(*node == NULL)
            return -1;

        updateHeight(*treeRoot);
        return insertToAvlList(&(*node)->listHead,patientRecord);
    }
    else if(cmpDates(newDate,&(*node)->nodeDate) == 0) { // a node with the same date already exists
        return insertToAvlList(&(*node)->listHead,patientRecord);
    }
    else if(cmpDates(newDate,&(*node)->nodeDate) > 0) { // new entry should go to the right subtree
        // insert to right subtree and rebalance if needed
        int retVal = insertToAvlTree(&(*node)->rChild,newDate,patientRecord,treeRoot);
        if(retVal)
            return -1;

        int heightDiff = getSubtreeHeightDiff(*node);

        if(heightDiff > 1 || heightDiff < -1) { // need to rebalance
            if(leftSubtreeBigger((*node)->rChild)) { // RL case
                printf("RL\n");
                rotateRight(treeRoot,(*node)->rChild);
                rotateLeft(treeRoot,*node);
            }
            else { // RR case
                printf("RR\n");
                rotateLeft(treeRoot,*node);
            }

            updateHeight(*treeRoot); // tree has been modified
        }
        return 0;
    }
    else if(cmpDates(newDate,&(*node)->nodeDate) < 0) { // new entry should go to the left subtree
        // insert to left subtree and rebalance if needed
        int retVal = insertToAvlTree(&(*node)->lChild,newDate,patientRecord,treeRoot);
        if(retVal)
            return -1;

        int heightDiff = getSubtreeHeightDiff(*node);

        if(heightDiff > 1 || heightDiff < -1) { // need to rebalance
            if(!leftSubtreeBigger((*node)->lChild)) { // LR case
                printf("LR\n");
                rotateLeft(treeRoot,(*node)->lChild);
                rotateRight(treeRoot,*node);
            }
            else { // LL case
                printf("LL\n");
                rotateRight(treeRoot,*node);
            }

            updateHeight(*treeRoot); // tree has been modified
        }
        return 0;
    }
    return 0;

}

bool leftSubtreeBigger(avlNode * node) {
    // this function will return true if new node got inserted in left subtree
    if(node->lChild == NULL) // definitely in left
        return false;

    if(node->rChild == NULL) // definitely in right
        return true;

    if(node->lChild->height > node->rChild->height)
        return true;

    return false;
}

int getSubtreeHeightDiff(avlNode * node) {
    // this function will return the difference of the subtree heights

    if(node->lChild == NULL && node->rChild == NULL)
        return 0;

    if(node->rChild == NULL)
        return node->lChild->height;

    if(node->lChild == NULL)
        return -node->rChild->height;

    return node->lChild->height - node->rChild->height;
}

void freeAvlList(listNode ** head) {
    if(*head != NULL) {
        freeAvlList(&(*head)->next);
        free(*head);
    }
}

void freeAvlTree(avlNode ** root) {
    if(*root != NULL) {
        freeAvlTree(&(*root)->lChild);
        freeAvlTree(&(*root)->rChild);

        freeAvlList(&(*root)->listHead);
        free(*root);
    }
}

void printAvlList(listNode * head) {
    if(head != NULL){
        printRecord(head->dataPointer);
        printAvlList(head->next);
    }
}

void printAvlTree(avlNode * root) {
    if(root != NULL) {
        printAvlTree(root->lChild);

        printf("\t*** NODE'S DATE: %d-%d-%d ***\n\t     | | |\n\t     v v v\n",\
        root->nodeDate.day,root->nodeDate.month,root->nodeDate.year);
        printAvlList(root->listHead);

        printAvlTree(root->rChild);
    }
}