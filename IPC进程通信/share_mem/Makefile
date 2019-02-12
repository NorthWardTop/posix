CC := gcc
CCFLG = -g -o
RM = rm -f
LIB= -pthread
SRC_SEND=shm_send.c myGeneral.c
SRC_RECV=shm_recv.c myGeneral.c
SEND = shmsend
RECV = shmrecv 


shmsend shmrecv:$(SRC_SEND) $(SRC_RECV)
	$(CC) $(CCFLG) $@ $^ $(LIB)



.PHONY:
clean:
	$(RM) $(SEND) $(RECV)
