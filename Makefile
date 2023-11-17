TARGET	= libmymalloc.a

SRC		= src/mymalloc.c

OBJ		= $(SRC:.c=.o)

CFLAGS	= -Wall -Wextra -Werror -I./include

all: $(TARGET)

$(TARGET): $(OBJ)
	ar rc $(TARGET) $(OBJ)

test: $(TARGET)
	rm -f ./test
	gcc $(CFLAGS) -o test main.c  -L. -lmymalloc

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)
	rm -f ./test
