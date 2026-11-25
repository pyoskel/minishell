# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/10/18 23:14:58 by pabartoc          #+#    #+#              #
#    Updated: 2026/11/25 20:11:16 by esttina          ###   ########.fr        #
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
			  $(SRC_DIR)/exec/builtins.c \
			  $(SRC_DIR)/exec/env.c \
			  $(SRC_DIR)/exec/redirs.c

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