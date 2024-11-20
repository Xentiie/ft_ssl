# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: reclaire <reclaire@student.42mulhouse.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/15 17:26:47 by reclaire          #+#    #+#              #
#    Updated: 2024/11/20 14:54:38 by reclaire         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.DEFAULT_GOAL=all
NAME=ft_ssl
CFLAGS=-Wall -Wextra -Wno-unknown-pragmas -g -O3
INCLUDES= -I./srcs/  -I./libft
LIBS= -lreadline  -lft -lm
LIBS_PATHS=  -L./libft
RM=rm -rf
CC=gcc
SRCS=./srcs/md5.c ./srcs/sha256.c ./srcs/main.c ./srcs/sha224.c ./srcs/sha1.c ./srcs/whirlpool.c
OBJS=./objs/md5.o ./objs/sha256.o ./objs/main.o ./objs/sha224.o ./objs/sha1.o ./objs/whirlpool.o
_libft: 
	$(MAKE) -C ./libft
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

./objs/md5.o: ./srcs/md5.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/md5.c -o ./objs/md5.o

./objs/sha256.o: ./srcs/sha256.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/sha256.c -o ./objs/sha256.o

./objs/main.o: ./srcs/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/main.c -o ./objs/main.o

./objs/sha224.o: ./srcs/sha224.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/sha224.c -o ./objs/sha224.o

./objs/sha1.o: ./srcs/sha1.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/sha1.c -o ./objs/sha1.o

./objs/whirlpool.o: ./srcs/whirlpool.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/whirlpool.c -o ./objs/whirlpool.o



$(NAME):	packages $(OBJS)
			$(CC)  $(INCLUDES) $(LIBS_PATHS) $(OBJS) $(LIBS) -o $(NAME)
