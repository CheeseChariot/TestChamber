
CC=gcc
EXEC=program.out
GRUPO=G1
NTAR=2
ARGS=./build/entrada.txt

SRC_DIR=src
OBJ_DIR=obj
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./incs/
#LIBS=
LIBS= -lm

CFLAGS=-Wall -Wextra -Wpedantic -O3
LDFLAGS= -Wall -lm 

all: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o build/$(EXEC) $(OBJ_FILES) $(INCLUDE) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^ $(INCLUDE)

.PHONY: clean folders send
clean:
	rm -f $(OBJ_FILES)
	rm -f build/$(EXEC)

dirs:
	mkdir -p src obj incs build docs
	touch src/.gitkeep obj/.gitkeep incs/.gitkeep build/.gitkeep docs/.gitkeep

send:
	tar czf $(GRUPO)-$(NTAR).tgz --transform 's,^,$(GRUPO)-$(NTAR)/,' Makefile src incs docs

run: all
	./build/$(EXEC)