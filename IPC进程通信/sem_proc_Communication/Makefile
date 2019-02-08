CC := gcc
CCFLG = -g -o
RM = rm -f
LIB= -pthread
SRC=nameless_sem_pthread.c
TARGET = pthread_namesem

$(TARGET):$(SRC)
	$(CC) $(CCFLG) $@ $^ $(LIB)

.PHONY:
clean:
	$(RM) $(TARGET)
