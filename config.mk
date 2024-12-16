.DEFAULT_GOAL=all
NAME=ft_ssl
CFLAGS=-Wall -Wextra -Wno-unknown-pragmas -g -O3
INCLUDES= -I./srcs/  -I/home/reclaire/Desktop/libft/include
LIBS=  -lft -lm
LIBS_PATHS=  -L/home/reclaire/Desktop/libft
RM=rm -rf
CC=afl-gcc
SRCS=./srcs/inputs/inputs.c ./srcs/digests.c ./srcs/main.c ./srcs/digests/md5.c ./srcs/digests/sha256.c ./srcs/digests/sha224.c ./srcs/digests/sha512.c ./srcs/digests/sha1.c ./srcs/digests/whirlpool.c ./srcs/ciphers/des_cbc.c ./srcs/ciphers/des_ecb.c ./srcs/ciphers/base64.c ./srcs/ciphers.c
OBJS=./objs/inputs.o ./objs/digests.o ./objs/main.o ./objs/md5.o ./objs/sha256.o ./objs/sha224.o ./objs/sha512.o ./objs/sha1.o ./objs/whirlpool.o ./objs/des_cbc.o ./objs/des_ecb.o ./objs/base64.o ./objs/ciphers.o
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

./objs/inputs.o: ./srcs/inputs/inputs.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/inputs/inputs.c -o ./objs/inputs.o

./objs/digests.o: ./srcs/digests.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests.c -o ./objs/digests.o

./objs/main.o: ./srcs/main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/main.c -o ./objs/main.o

./objs/md5.o: ./srcs/digests/md5.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests/md5.c -o ./objs/md5.o

./objs/sha256.o: ./srcs/digests/sha256.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests/sha256.c -o ./objs/sha256.o

./objs/sha224.o: ./srcs/digests/sha224.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests/sha224.c -o ./objs/sha224.o

./objs/sha512.o: ./srcs/digests/sha512.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests/sha512.c -o ./objs/sha512.o

./objs/sha1.o: ./srcs/digests/sha1.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests/sha1.c -o ./objs/sha1.o

./objs/whirlpool.o: ./srcs/digests/whirlpool.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/digests/whirlpool.c -o ./objs/whirlpool.o

./objs/des_cbc.o: ./srcs/ciphers/des_cbc.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/ciphers/des_cbc.c -o ./objs/des_cbc.o

./objs/des_ecb.o: ./srcs/ciphers/des_ecb.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/ciphers/des_ecb.c -o ./objs/des_ecb.o

./objs/base64.o: ./srcs/ciphers/base64.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/ciphers/base64.c -o ./objs/base64.o

./objs/ciphers.o: ./srcs/ciphers.c
	$(CC) $(CFLAGS) $(INCLUDES) -c ./srcs/ciphers.c -o ./objs/ciphers.o

