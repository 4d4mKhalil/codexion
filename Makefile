NAME = codexion

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = src/coder.c src/dongle.c src/init.c src/log.c src/main.c src/min_heap.c src/monitor.c src/parse.c src/time_utils.c src/utils.c

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
