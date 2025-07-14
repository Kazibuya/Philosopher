NAME		= philo

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g -pthread -Iincludes

SRCS_DIR	= srcs
OBJS_DIR	= objs

SRCS		= $(addprefix $(SRCS_DIR)/, init.c dying.c launch.c main.c print.c)
OBJS		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
