.DEFAULT_GOAL=all
NAME=ft_ssl
CFLAGS=-Wall -Wextra -Wno-unknown-pragmas -g
INCLUDES= -I./srcs/  -I/mnt/c/Users/reclaire/Desktop/libft
LIBS=  -lft -lm
LIBS_PATHS=  -L/mnt/c/Users/reclaire/Desktop/libft
RM=rm -rf
CC=gcc
SRCS=./srcs/main.c ./srcs/md5.c
OBJS=./objs/main.o ./objs/md5.o
_libft: 
	$(MAKE) -C ../libft
.PHONY: _libft

packages: _libft
.PHONY: packages

all: objs packages $(NAME)
.PHONY: all

objs: 
	mkdir -p ./objs

clean: 
	$(RM) $(OBJS)
.PHONY: clean

fclean: clean
	$(RM) $(NAME)
.PHONY: fclean

re: fclean all
.PHONY: re

./objs/main.o: ./srcs/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/main.c -o ./objs/main.o

./objs/md5.o: ./srcs/md5.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/md5.c -o ./objs/md5.o

