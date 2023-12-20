NAME := philo

LIB := philosophers.h

FILES := ft_atoi.c ft_isint.c main.c tasks.c tools.c
OFILES := $(FILES:%.c=%.o)

COMPILER := cc

LDFLAGS := -Wall -Wextra -Werror -g -fsanitize=thread
CFLAGS := -Wall -Wextra -Werror -g -fsanitize=thread
ADDITIONAL :=

all: $(NAME)

$(NAME): $(OFILES) $(LIB) Makefile
	$(COMPILER) $(LDFLAGS) $(OFILES) -o $@ 
 
%.o: %.c $(LIB) Makefile
	$(COMPILER) $(CFLAGS) $< -c

clean:
	rm -rf $(OFILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all

bonus:

.PHONY: all clean fclean re bonus
