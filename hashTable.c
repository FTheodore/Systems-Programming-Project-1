//
// Created by theo on 13/3/20.
//

#include "hashTable.h"

hashTable * initHashTable(int tableSize, int bucketSize) { // initialize a new hash table
    hashTable * newTable;

    newTable = malloc(sizeof(hashTable));
    if(memErrCheck(newTable))
        return NULL;

    newTable->tableSize = tableSize;
    newTable->maxEntriesInBucket = (bucketSize - sizeof(listNode *)) / sizeof(bucketEntry);
    // now allocate the actual array
    newTable->table = malloc(tableSize * sizeof(listNode *));
    if(memErrCheck(newTable->table))
        return NULL;

    for (int i = 0; i < tableSize; ++i) {
        newTable->table[i] = NULL;
    }

    return newTable;
}

listNode * initBucketNode(int maxEntriesInBucket) { // initialize a new bucket node
    listNode * newNode;

    newNode = malloc(sizeof(listNode));
    if(memErrCheck(newNode))
        return NULL;

    newNode->next = NULL;
    newNode->dataPointer = malloc(sizeof(bucketNode));
    if(memErrCheck(newNode->dataPointer))
        return NULL;

    ((bucketNode *)newNode->dataPointer)->arrayOfEntries = malloc(maxEntriesInBucket * sizeof(bucketEntry));
    if(memErrCheck(((bucketNode *)newNode->dataPointer)->arrayOfEntries))
        return NULL;

    for (int i = 0; i < maxEntriesInBucket; ++i) {
        ((bucketNode *)newNode->dataPointer)->arrayOfEntries->string = NULL;
        ((bucketNode *)newNode->dataPointer)->arrayOfEntries->avlPtr = NULL;
    }

    ((bucketNode *)newNode->dataPointer)->count = 0;

    return newNode;
}

int idToInt(char const * string) { // get arithmetic key value from an id in order to hash
    int sum = 0;
    int j = 0;
    for(int i = ((int)strlen(string) - 1); i >= 0; --i) {
        sum += string[i] * pow(2, j);
        j++;
    }
    return sum;
}

int hashFunction(char const * string, int tableSize) {
    int key = idToInt(string);

    return (2 * key + 1) % tableSize;
}

bool bucketEntryExists(char const * newString, listNode * head,\
listNode ** retAddress, int maxEntries, bool * noSpace) {
    // check if the same entry already exists in bucket and return the appropriate memory address
    int entriesInNode = ((bucketNode *)head->dataPointer)->count;
    for (int i = 0; i < entriesInNode; ++i) {
        if(strcmp(((bucketNode *)head->dataPointer)->arrayOfEntries[i].string,newString) == 0) { // entry exists
            *retAddress = head;
            return true;
        }
    }

    if(head->next != NULL) {
        return bucketEntryExists(newString,head->next,retAddress,maxEntries,noSpace);
    }
    else { // reached end of bucket, entry not found
        *retAddress = head;

        if(entriesInNode == maxEntries) // inform whether current node has space for a new entry
            *noSpace = true;
        else
            *noSpace = false;

        return false;
    }

}

int insertToBucket(bucketNode * node, char * newString, date * newDate, listNode * recordPointer) {
    int retVal;

    for (int i = 0; i < node->count; ++i) { // check if same bucket entry already exists
        if(strcmp(newString,node->arrayOfEntries[i].string) == 0) {
            retVal = insertToAvlTree(&node->arrayOfEntries[i].avlPtr,newDate,\
            recordPointer,&node->arrayOfEntries[i].avlPtr);
            if(retVal)
                return -1;

            return 0;
        }
    }

    // new entry must be created
    node->arrayOfEntries[node->count].string = strdup(newString);
    if(memErrCheck(node->arrayOfEntries[node->count].string))
        return -1;

    retVal = insertToAvlTree(&node->arrayOfEntries[node->count].avlPtr,newDate,\
    recordPointer,&node->arrayOfEntries[node->count].avlPtr);
    if(retVal)
        return -1;

    node->count += 1;
    return 0;
}

int insertToHashTable(hashTable * hashT, char * newString, date * newDate, listNode * recordPointer) {
    // first hash the given string
    int index = hashFunction(newString,hashT->tableSize);

    //current bucket is empty
    if(hashT->table[index] == NULL) {
        hashT->table[index] = initBucketNode(hashT->maxEntriesInBucket);
        if(hashT->table[index] == NULL)
            return -1;

        // insert the entry to the new bucket
        return insertToBucket(hashT->table[index]->dataPointer,newString,newDate,recordPointer);
    }

    listNode * nodeToInsert;
    bool noSpace;
    bool entryExists = bucketEntryExists(newString,hashT->table[index],\
    &nodeToInsert,hashT->maxEntriesInBucket,&noSpace);

    if(entryExists || !noSpace) {
        return insertToBucket(nodeToInsert->dataPointer,newString,newDate,recordPointer);
    }
    else { // need to create new bucket node because the bucket node is full
       nodeToInsert->next = initBucketNode(hashT->maxEntriesInBucket);
       if(nodeToInsert->next == NULL)
           return -1;

        return insertToBucket(nodeToInsert->next->dataPointer,newString,newDate,recordPointer);
    }
}

void freeBucketList(listNode ** head) {
    if(*head != NULL) {
        freeBucketList(&(*head)->next);

        for (int i = 0; i < ((bucketNode *)(*head)->dataPointer)->count; ++i) {
            freeAvlTree(&((bucketNode *)(*head)->dataPointer)->arrayOfEntries[i].avlPtr);
            free(((bucketNode *)(*head)->dataPointer)->arrayOfEntries[i].string);
        }

        free(((bucketNode *)(*head)->dataPointer)->arrayOfEntries);
        free((*head)->dataPointer);
        free(*head);
    }
}

void freeHashTable(hashTable ** hashT) {
    for (int i = 0; i < (*hashT)->tableSize; ++i) {
        if((*hashT)->table[i] != NULL)
            freeBucketList(&(*hashT)->table[i]);
    }

    free(*hashT);
}

void printHashList(listNode * head) {
    if(head != NULL){
        printf("(start of bucket node)\n");
        for (int i = 0; i < ((bucketNode *)head->dataPointer)->count; ++i) {
            printf("$$$  %s  $$$\n",((bucketNode *)head->dataPointer)->arrayOfEntries[i].string);
            printAvlTree(((bucketNode *)head->dataPointer)->arrayOfEntries[i].avlPtr);
        }
        printf("(end of bucket node)\n");

        printHashList(head->next);
    }
}

void printHashTable(hashTable * hashT) {
    printf("<<<< PRINTING HASH TABLE >>>>\n");
    for (int i = 0; i < hashT->tableSize; ++i) {
        if(hashT->table[i] != NULL) {
            printf("\t - IN TABLE INDEX %d -\n",i);
            printHashList(hashT->table[i]);
        }
    }
    printf("\n<<<<< EOT >>>>>>\n\n");
}

void countHashListEntries(listNode * head, date start, date end, bool datesGiven) {
    // count number of cases for every disease in a bucket
    if(head != NULL) {
        bucketNode * nodeData = head->dataPointer;
        for (int i = 0; i < nodeData->count; ++i) {
            int count = 0;
            countAvlTreeEntries(nodeData->arrayOfEntries[i].avlPtr,&count,datesGiven,false,NULL,start,end);
            printf("Disease: %s , Number of cases: %d\n",nodeData->arrayOfEntries[i].string,count);
        }
        countHashListEntries(head->next,start,end,datesGiven);
    }
}

void countHashListSick(listNode * head) {
    // count number of not treated cases for every disease in a bucket
    if(head != NULL) {
        bucketNode * nodeData = head->dataPointer;
        for (int i = 0; i < nodeData->count; ++i) {
            int count = 0;
            countAvlTreePatients(nodeData->arrayOfEntries[i].avlPtr,&count);
            printf("Disease: %s , Number of sick people: %d\n",nodeData->arrayOfEntries[i].string,count);
        }
        countHashListSick(head->next);
    }
}

void findCountForDisease(listNode * head, char * virusName, bool countryGiven, char *country, date start, date end) {
    // count number of reported cases for a specific disease in a bucket
    if(head != NULL) {
        bucketNode * nodeData = head->dataPointer;
        for (int i = 0; i < nodeData->count; ++i) {
            if(strcmp(nodeData->arrayOfEntries[i].string,virusName) == 0) {
                int count = 0;
                countAvlTreeEntries(nodeData->arrayOfEntries[i].avlPtr,&count,true,countryGiven,country,start,end);
                printf("Number of cases for virus %s are %d\n",nodeData->arrayOfEntries[i].string,count);
                return;
            }
        }
        findCountForDisease(head->next,virusName,countryGiven,country,start,end);
    }
    else
        printf("No cases exist for this disease\n");
}

void findPatientsForDisease(listNode * head, char * virusName) {
    // count number of not treated cases for a specific disease in a bucket
    if(head != NULL) {
        bucketNode * nodeData = head->dataPointer;
        for (int i = 0; i < nodeData->count; ++i) {
            if(strcmp(nodeData->arrayOfEntries[i].string,virusName) == 0) {
                int count = 0;
                countAvlTreePatients(nodeData->arrayOfEntries[i].avlPtr,&count);
                printf("Number of sick people for virus %s are %d\n",nodeData->arrayOfEntries[i].string,count);
                return;
            }
        }
        findPatientsForDisease(head->next,virusName);
    }
    else
        printf("No cases exist for this disease\n");
}

int findForHeap(listNode * head, char * string,int numOfValues,char type,bool datesGiven,date start,date end) {
    // find a specific disease or country in a bucket and call max heap in order to get top N countries/diseases
    if(head != NULL) {
        bucketNode * nodeData = head->dataPointer;
        for (int i = 0; i < nodeData->count; ++i) {
            if(strcmp(nodeData->arrayOfEntries[i].string,string) == 0) {
                int retVal = getTopValues(numOfValues,nodeData->arrayOfEntries[i].avlPtr,type,datesGiven,start,end);
                return retVal;
            }
        }
        return findForHeap(head->next,string,numOfValues,type,datesGiven,start,end);
    }
    else {
        printf("No cases exist for %s\n",string);
        return 0;
    }
}

void getAllDiseaseStats(hashTable * diseaseTbl, bool datesGiven, date start, date end) {
    // count number of cases for every disease in the hash table
    for (int i = 0; i < diseaseTbl->tableSize; ++i) {
        countHashListEntries(diseaseTbl->table[i],start,end,datesGiven);
    }
}

void getAllSickPatients(hashTable * diseaseTbl) {
    // count number of not treated cases for every disease in the hash table
    for (int i = 0; i < diseaseTbl->tableSize; ++i) {
        countHashListSick(diseaseTbl->table[i]);
    }
}

void getDiseaseSickPatients(hashTable * diseaseTbl, char * virusName) {
    // count number of not treated cases for a specific disease in the hash table
    int index = hashFunction(virusName,diseaseTbl->tableSize);

    findPatientsForDisease(diseaseTbl->table[index],virusName);
}

void getDiseaseStats(hashTable * diseaseTbl, char * virusName, bool countryGiven, char *country, date start, date end) {
    // count number of cases for a specific disease in the hash table
    int index = hashFunction(virusName,diseaseTbl->tableSize);

    findCountForDisease(diseaseTbl->table[index],virusName,countryGiven,country,start,end);
}

int getEntryTopValues(hashTable * hashT,char * string,int numOfValues,char type,bool datesGiven,date start,date end) {
    int index = hashFunction(string,hashT->tableSize);

    return findForHeap(hashT->table[index],string,numOfValues,type,datesGiven,start,end);
}