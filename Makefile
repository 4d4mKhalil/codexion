NAME = codexion

CFLAGS = -Wall -Wextra -Werror -pthread

SRC = cdx/coder.c cdx/dongle.c cdx/init.c cdx/log.c cdx/main.c cdx/min_heap.c cdx/monitor.c cdx/parse.c cdx/time_utils.c cdx/utils.c

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
