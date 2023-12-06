NAME := philo

LIB := philosophers.h philosophers_bonus.h

FILES := ft_atoi.c ft_isint.c main.c tasks.c tools.c
OFILES := $(FILES:%.c=%.o)

COMPILER := cc

FLAGS := -Wall -Wextra -Werror -fsanitize=thread -g
ADDITIONAL := 

all: $(NAME)

$(NAME): $(OFILES)
	$(COMPILER) $(FLAGS) $^ $(ADDITIONAL) -o $@

%.o: %.c $(LIB)
	$(COMPILER) $(FLAGS) $< -c

clean:
	rm -rf $(OFILES)

fclean: clean
	rm -rf $(NAME)

re: fclean all

bonus:

.PHONY: all clean fclean re bonus
