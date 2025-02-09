CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBDIR = libft
LIB = $(LIBDIR)/libft.a

OBJS = $(SRC:.c=.o)
SRC = main.c free_exit.c
NAME = pipex

all:  $(NAME)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(LIB):
	$(MAKE) -C $(LIBDIR)
$(NAME): $(OBJS) $(LIB)
	$(CC) $(OBJS) -L$(LIBDIR) -lft -o $(NAME)


clean:
	rm -rf $(OBJS)
	$(MAKE) clean -C $(LIBDIR)

fclean: clean
	$(MAKE) fclean -C $(LIBDIR)
	rm -f $(NAME)

re: fclean all
.PHONY: all clean fclean re
