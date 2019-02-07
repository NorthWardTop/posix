.PHONY:all
all:client server	# 要生成的目标

# $@(自动目标文件名) $^(自动所有依赖文件名) $<(第一个依赖的文件名) 
client:client.c msqid_ds.c myGeneral.c
	gcc -g -o $@ $^

server:server.c msqid_ds.c myGeneral.c
	gcc -g -o $@ $^

