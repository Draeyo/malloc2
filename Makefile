ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

SYMLINK = libft_malloc.so

SRC_FILES = malloc.c alloc_zone.c tools.c free.c calloc.c realloc.c show_alloc_mem.c align.c \
alloc_small.c alloc_tiny.c alloc_large.c

SRC_PATH = srcs/

SRC = $(addprefix $(SRC_PATH),$(SRC_FILES))

OBJ = $(SRC:.c=.o)

LIB_NAME = -lft

LIB_PATH = libft/

INC = includes/

CC = gcc

CFLAGS = -g -Wall -Wextra -Werror #-fsanitize=address

RED = \033[31;44m
GREEN = \033[31;44m
ENDC = \033[0m

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	@make -s -C $(LIB_PATH)
	@$(CC) $(CFLAGS) -shared -o $@ $(OBJ) ./libft/libft.a
	@ln -sF $(NAME) $(SYMLINK)

%.o: %.c
	@$(CC) $(CFLAGS) -I $(INC) -o $@ -c $<

clean:
	@(rm -f $(OBJ))

fclean: clean
	@(rm -f $(NAME) $(SYMLINK))
	@make -s -C $(LIB_PATH) fclean

re: fclean all
