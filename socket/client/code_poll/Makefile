#编译器为 gcc
CC = g++ 
CCFLAG = -g  -o

RM = rm -f

#应用程序目标文件
OBJGEN = server_poll 

#获取所在目录下所有c源文件
SRC = $(wildcard *.c)  

#所有的.c 换成 .o 
OBJS = $(patsubst %.c ,%.o,$(SRC)) 

# $@ $^ $< 是自动变量
# S@ :表示规则的目标文件名
# $^ :表示所有依赖的文件名
# $< :表示第一个依赖的文件名
$(OBJGEN):$(OBJS)
	$(CC) $(CCFLAG) $@ $^
	
.PHONY:
clean:
	$(RM) $(OBJS) 