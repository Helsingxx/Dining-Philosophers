NAME := philo_bonus

LIB := philosophers_bonus.h

FILES := ft_atoi.c ft_isint.c main_bonus.c tasks_bonus.c
OFILES := $(FILES:%.c=%.o)

COMPILER := cc

LDFLAGS := -Wall -Wextra -Werror -g -fsanitize=thread
CFLAGS := -Wall -Wextra -Werror -g -fsanitize=thread
VPATH := ..
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


.PHONY: all clean fclean re bonus
