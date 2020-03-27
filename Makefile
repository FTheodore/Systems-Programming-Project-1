OBJS 	= main.o avlTree.o binaryHeap.o hashTable.o miscFunctions.o patientList.o
SOURCE	= main.c avlTree.c binaryHeap.c hashTable.c miscFunctions.c patientList.c
HEADER	= structs.h avlTree.h binaryHeap.h hashTable.h miscFunctions.h patientList.h
OUT  	= diseaseMonitor
CC	= gcc
FLAGS   = -c -Wall

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@ -lm

main.o: main.c
	$(CC) $(FLAGS) main.c

avlTree.o: avlTree.c
	$(CC) $(FLAGS) avlTree.c

binaryHeap.o: binaryHeap.c
	$(CC) $(FLAGS) binaryHeap.c

hashTable.o: hashTable.c
	$(CC) $(FLAGS) hashTable.c

miscFunctions.o: miscFunctions.c
	$(CC) $(FLAGS) miscFunctions.c

patientList.o: patientList.c
	$(CC) $(FLAGS) patientList.c

clean:
	rm -f $(OBJS) $(OUT)
