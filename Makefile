# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 12:45:51 by mdesoeuv          #+#    #+#              #
#    Updated: 2022/01/26 09:55:30 by vchevill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc -Wall -Werror -Wextra -g -fsanitize=address -g3

SRCS =	main.c \
		built_in_pwd.c \
		built_in_cd.c \
		built_in_unset.c \
		built_in_export.c \
		built_in_env.c \
		built_in_getenv.c \
		parsing.c \
		ft_free.c \
		built_in_echo.c \
		built_in_exit.c \
		signal.c \
		parsing_chevron.c \
		parsing_variable.c \
		ft_utils.c \
		pipe1.c \
		pipe2.c \
		exec.c \
		exec_builtin.c \
		cmd_process.c \
		debug_functions.c \
		heredoc.c

SRCS_FILES = $(addprefix srcs/, $(SRCS))

OBJS = $(SRCS:.c=.o)

OBJS_FILES = $(addprefix objs/, $(OBJS))

LIB = libft/libft.a

all : libft $(NAME)

$(NAME)	:	$(OBJS_FILES) $(LIB) Makefile
			$(CC) $(OBJS_FILES) -o $(NAME) libft/libft.a -lreadline

libft	:	
			$(MAKE) -C libft

objs/%.o:	srcs/%.c	srcs/minishell.h libft/libft.h
			@mkdir -p objs
			$(CC) -c $< -o $@ 
			
clean	:
			rm -rf objs/
			rm -rf tmp/
			make clean -C libft
			
fclean	:	clean
			rm -f $(NAME)
			make fclean -C libft

re		:	fclean all

.PHONY	:	libft all clean re fclean
