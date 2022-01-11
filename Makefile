# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/04 12:45:51 by mdesoeuv          #+#    #+#              #
#    Updated: 2022/01/11 16:52:43 by vchevill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

USER = mdesoeuv

CC = gcc -Wall -Werror -Wextra

SRCS =	readline_tests.c \
		built_in_pwd.c \
		built_in_cd.c \
		parsing.c \
		ft_free.c \
		built_in_echo.c \
		built_in_exit.c \
		pipe.c \
		print_split.c \
		signal.c \
		parsing_chevron.c \
		parsing_variable.c

SRCS_FILES = $(addprefix srcs/, $(SRCS))

OBJS = $(SRCS:.c=.o)

OBJS_FILES = $(addprefix objs/, $(OBJS))

LIB = libft/libft.a

all : libft $(NAME)

$(NAME)	:	$(OBJS_FILES) $(LIB)
			$(CC) -fsanitize=address -g3 $(OBJS_FILES) -o $(NAME) libft/libft.a -lreadline -L /Users/$(USER)/homebrew/opt/readline/lib -I/Users/$(USER)/homebrew/opt/readline/include 

libft	:	
			$(MAKE) -C libft

objs/%.o:	srcs/%.c	srcs/minishell.h libft/libft.h
			@mkdir -p objs
			$(CC) -c $< -o $@
			
clean	:
			rm -rf objs/
			make clean -C libft
			
fclean	:	clean
			rm -f $(NAME)
			make fclean -C libft

re		:	fclean all

.PHONY	:	libft all clean re fclean
			