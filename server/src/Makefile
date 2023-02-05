#通用Makefile
out:=../bin/server#可执行文件名称需要根据需要更改
SRCS:=$(wildcard *.cc)#拿到当前目录下所有的.c文件
#OBJS:=$(patsubst %.cc,%.o,$(SRCS))#把.c文件替换为.o文件
#预定义变量
CC:=g++
# CFLAGS:=-g
$(out):$(SRCS)
	$(CC) $^ -o $@  -pthread -llog4cpp -ltinyxml2 -I ../include -g
# %.o:%.cc
	# $(CC) -c $^ -o $@

#.PHONY:clean rebuild
rebuild:clean $(out)
clean:
	rm -rf $(OBJS) $(out)
