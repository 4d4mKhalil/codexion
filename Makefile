NAME = codexion

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = coder.c dongle.c init.c log.c main.c min_heap.c monitor.c parse.c time_utils.c utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	cc $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
