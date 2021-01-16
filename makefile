APP_NAME = rotate

CC = gcc
CFLAGS = --std=c18  -pedantic -Wall -Werror

all: $(APP_NAME)

define o_build
	$(CC) -c $(CFLAGS) $< -o $@ 
endef

main.o: main.c
	$(call o_build) 

img_rotation.o: img_rotation.c img_rotation.h
	$(call o_build) 

file_io.o: file_io.c file_io.h
	$(call o_build) 

bmp_io.o: bmp_io.c bmp_io.h
	$(call o_build) 

$(APP_NAME): main.o img_rotation.o file_io.o bmp_io.o
	$(CC) -o $(APP_NAME) $^

clean:
	rm -f main.o img_rotation.o file_io.o bmp_io.o $(APP_NAME)

