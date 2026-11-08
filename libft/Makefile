# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/08 22:11:45 by pabartoc          #+#    #+#              #
#    Updated: 2026/11/08 16:46:59 by pabartoc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = libft.a

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I $(INC_DIR) -I $(PRINTF_INC) -I $(GNL_DIR)
AR          = ar rcs
RM          = rm -f

# ---------------------------------------------------------
# Directory-structur
# ---------------------------------------------------------
SRC_DIR     = src
INC_DIR     = inc
OBJ_DIR     = obj
PRINTF_DIR  = ft_printf/src
PRINTF_INC  = ft_printf/inc
GNL_DIR     = get_next_line

# ---------------------------------------------------------
# DATEIEN (Nur die Namen, OHNE Pfad)
# ---------------------------------------------------------
FILES = ft_atoi.c \
		ft_bzero.c \
		ft_calloc.c \
		ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_itoa.c \
		ft_lstadd_front.c \
		ft_lstadd_back.c \
		ft_lstclear.c \
		ft_lstdelone.c \
		ft_lstiter.c \
		ft_lstlast.c \
		ft_lstmap.c \
		ft_lstnew.c \
		ft_lstsize.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_memset.c \
		ft_putchar_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_putstr_fd.c \
		ft_split.c \
		ft_strchr.c \
		ft_strdup.c \
		ft_striteri.c \
		ft_strjoin.c \
		ft_strlcat.c \
		ft_strlcpy.c \
		ft_strlen.c \
		ft_strmapi.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_strrchr.c \
		ft_strtrim.c \
		ft_substr.c \
		ft_tolower.c \
		ft_toupper.c

PRINTF_FILES = ft_printf.c \
			   ft_printf_add_ons.c

# We use the bonus files from get_next_line for libft.a, since they
# support both single and multiple file descriptors and
# prevent duplicate function names (duplicate symbols) during linking.
GNL_FILES    = get_next_line_bonus.c \
			   get_next_line_utils_bonus.c

# ---------------------------------------------------------
# PFADE AUTOMATISCH GENERIEREN
# ---------------------------------------------------------
SRC        = $(addprefix $(SRC_DIR)/, $(FILES))
PRINTF_SRC = $(addprefix $(PRINTF_DIR)/, $(PRINTF_FILES))
GNL_SRC    = $(addprefix $(GNL_DIR)/, $(GNL_FILES))

OBJ        = $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o)) \
			 $(addprefix $(OBJ_DIR)/, $(PRINTF_FILES:.c=.o)) \
			 $(addprefix $(OBJ_DIR)/, $(GNL_FILES:.c=.o))

# ---------------------------------------------------------
# Default rule
# ---------------------------------------------------------
all: $(NAME)

# Create the static library
$(NAME): $(OBJ)
	@$(AR) $(NAME) $(OBJ)
	@echo
	@echo
	@echo "\033[1;32m ✅ [=> '$(NAME)' (inkl. ft_printf & get_next_line) successfully created!]\033[0m"
	@echo
	@echo

# Compilation Rules
# $< is the .c file, $@ is the .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PRINTF_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	@$(RM) -r $(OBJ_DIR)
	@echo
	@echo
	@echo "\033[1;35m ✅🧹 [=> Object directory cleaned up.]\033[0m"
	@echo
	@echo

# Clean everything (object files and library)
fclean:
	@$(RM) -r $(OBJ_DIR)
	@$(RM) $(NAME)
	@echo
	@echo
	@echo "\033[1;35m ✅🧹 [=> Object directory cleaned up.]\033[0m"
	@echo "\033[1;35m ✅🧹 [=> '$(NAME)' deleted.]\033[0m"
	@echo
	@echo

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re