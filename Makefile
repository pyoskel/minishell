# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/18 23:14:58 by pabartoc          #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2026/11/24 04:41:33 by pabartoc         ###   ########.fr        #
=======
#    Updated: 2026/11/24 05:27:49 by esttina          ###   ########.fr        #
>>>>>>> refs/remotes/github/master
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc -Ilibft/inc

# Directory-structur
SRC_DIR     = src
INC_DIR     = inc
LIBFT_DIR   = libft

# The Library
LIBFT       = $(LIBFT_DIR)/libft.a

# Source Files and Object Files
SRCS        = $(SRC_DIR)/minishell.c \
			  $(SRC_DIR)/exec/executor.c \
			  $(SRC_DIR)/parsing/parser.c \
			  $(SRC_DIR)/parsing/tokens_list.c \
<<<<<<< HEAD
			  $(SRC_DIR)/parsing/syntax_check.c
=======
			  $(SRC_DIR)/exec/builtins.c \
			  $(SRC_DIR)/exec/env.c
>>>>>>> refs/remotes/github/master

OBJS        = $(SRCS:.c=.o)

# Default rule
all: $(NAME)

# Build the minishell programm
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Build the libft
$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	@make -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re