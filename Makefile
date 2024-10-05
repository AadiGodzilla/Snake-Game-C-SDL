CC=gcc
CFLAGS=-Wall
LIBS=-lSDL2 -lSDL2main -lSDL2_ttf -lSDL2_image
SRC_DIR=src
OBJ_DIR=obj
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILE=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
EXE=Snake

target: $(OBJ_FILE)
	$(CC) $(CFLAGS) -g -o $(EXE) $(OBJ_FILE) $(LIBS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ_DIR)/*.o
