CC = gcc
CCFLAG = -g
TARGET = pthread_pool
SRC = pthread_pool.c base.c
OBJECT = pthread_pool.o  base.o
INCLUDES = -I./
LDFLAGS = -lpthread


all:$(TARGET) client

$(OBJECT):$(SRC)
	$(CC) $(CCFLAG)  -c $(INCLUDES) ${SRC}

$(TARGET):$(OBJECT)
	$(CC) $(CCFLAG) -o $@ $(OBJECT) $(LDFLAGS)

client:client.c
	$(CC) $(CCFLAG) -o $@ $^

.PHONY:clean
clean:
	@rm -rf $(OBJECT) $(TARGET) *~

