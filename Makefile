# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 12:45:51 by mdesoeuv          #+#    #+#              #
#    Updated: 2022/01/04 12:59:59 by mdesoeuv         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -Wall -Werror -Wextra -fsanitize=address -g3

SRCS = main.c

SRCS_PATH = srcs/

SRCS_FILES = $(addprefix srcs/, $(SRCS))

OBJS = $(SRCS:.c=.o)

OBJS_FILES = $(addprefix objs/, $(OBJS))

objs/%.o :	srcs/%.c	srcs/minishell.h libft/libft.h
			@mkdir -p objs
			$(CC) -c $< -o $@

all : $(NAME) libft

libft : 
		make -C libft

$(NAME) :	$(OBJS_FILES) libft/libft.a
			$(CC) $(OBJS_FILES) libft/libft.a
			
clean	:
			rm -rf objs/
			make clean -C libft
			
fclean	:	clean
			rm -f $(NAME)
			make fclean -C libft

re		:	fclean all

.PHONY	:	all, clean, re, libft, fclean
			