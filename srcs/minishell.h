/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/10 17:20:59 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
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

/*

typedef struct s_shell
{
	int						pipes_nbr;
	struct s_list_pipes		*list_start;
}	t_shell;

*/

/*

typedef struct s_list_pipes
{
	int					*pipe_fd[2];
	char				**command;
	char				*cmd_path;
	char				**file_in;
	int					file_in_overwrite;
	int					fd_file_in;
	char				**file_out;
	int					file_out_overwrite;
	int					fd_file_out;
	struct s_list_pipes	*next;
}	t_list_pipes;

*/

/* BUILT-IN */

void	print_working_directory(void);
char	*return_working_directory(void);
int		change_directory(char *arg);
void	ft_parsing(char *line, t_shell	*shell);
void	ft_free(char *message, t_shell	*shell, int is_error);
void	ft_echo(t_shell *shell);
int		ft_exit(t_shell *shell);
int		ft_parse_quotes(int i, int index_start,
			char quote_type, t_shell *shell);
#endif
