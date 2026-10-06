# **************************************************************************** #
#                             CODEXION - MAKEFILE                              #
# **************************************************************************** #

NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread
INC_DIR     = includes
SRC_DIR     = src

SRCS        = $(shell find ./src -name "*.c" | sort)
OBJS        = $(SRCS:.c=.o)

INCLUDES    = -I$(INC_DIR)

# **************************************************************************** #
#                                  RULES                                       #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/codexion.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

tsan:
	$(CC) $(CFLAGS) $(INCLUDES) -fsanitize=thread -g3 $(SRCS) -o $(NAME)

asan:
	$(CC) $(CFLAGS) $(INCLUDES) -fsanitize=address -g3 $(SRCS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
