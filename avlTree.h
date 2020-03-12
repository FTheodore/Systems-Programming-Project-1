//
// Created by theo on 12/3/20.
//

#ifndef SYSPRO_1_AVLTREE_H
#define SYSPRO_1_AVLTREE_H

#include "stdbool.h"
#include "miscFunctions.h"

avlNode * initAvlNode(date const * nodeDate);
listNode * initListNodeOfAvl(listNode * patientRecord);
int getSubtreeHeightDiff(avlNode * node);
bool leftSubtreeBigger(avlNode * node);
int insertToAvlList(listNode ** listHead, listNode * patientRecord);
int insertToAvlTree(avlNode ** node, date * newDate, listNode * patientRecord, avlNode * treeRoot);
void rotateLeft(avlNode **root,avlNode *node);
void rotateRight(avlNode **root,avlNode *node);
void updateHeight(avlNode * node);
void freeAvlList(listNode ** head);
void freeAvlTree(avlNode ** root);

#endif //SYSPRO_1_AVLTREE_H