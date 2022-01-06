/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/06 19:25:41 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include "../libft/libft.h"

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef struct s_cmd
{
	char	*cmd_path;
	char	**cmd_arg;
	char	*cmd_in;
	char	*cmd_out;
}	t_cmd;

typedef struct s_cmd
{
	char	*cmd_path;
	char	**cmd_arg;
	char	*cmd_in;
	char	*cmd_out;
}	t_cmd;

typedef struct s_list_semicolons
{
	struct s_list_pipes			*pipes_list;
	struct s_list_semicolons	*next;
}	t_list_semicolons;

typedef struct s_list_pipes
{
	char					**name;
	struct s_list_pipe_args	**args;
	char					**filein;
	char					**fileout;
	struct s_list_pipes		*next;
}	t_list_pipes;

typedef struct s_list_pipe_args
{
	char					**name;
	struct s_list_pipe_args	*next;
}	t_list_pipe_args;

void	print_working_directory(void);
char	*return_working_directory(void);
int		change_directory(char *arg);

#endif
