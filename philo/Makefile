# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radubos <radubos@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/24 18:05:45 by radubos           #+#    #+#              #
#    Updated: 2025/08/21 17:25:20 by radubos          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Project name
NAME = philo

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -pthread -fsanitize=address

# Source files and object files
SRC = main.c init.c utils.c monitor.c routine.c routine_actions.c check.c \
init_data.c init_philo.c routine_time.c
OBJ = $(SRC:.c=.o)

# Default rule
all: $(NAME)

# Compilation rules
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	
# Cleaning rules
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re