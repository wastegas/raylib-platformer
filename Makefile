TARGET	=	game
CC	=	c++
CXXFLAGS	=	`pkg-config --cflags raylib` -Wall  -std=c++17
INCLUDE	=	-Iinclude

SRC_DIR	=	src
OBJ_DIR	=	build
SRCS	=	$(wildcard $(SRC_DIR)/*.cpp)
OBJS	=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))


LDFLAGS	=	`pkg-config --libs raylib` -lm

.PHONY:	all clean run

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)
