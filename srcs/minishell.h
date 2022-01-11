/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/11 11:40:20 by mdesoeuv         ###   ########lyon.fr   */
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

/* PIPE */

void	concatenate_path(t_list_pipes *pipe_lst, char *path);
void	error_cmd_not_found(char **cmd);
void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst);
int		manage_file_fd(t_shell *shell, t_list_pipes *pipe_lst, int i);
int		manage_dup_fd(t_shell *shell, t_list_pipes *pipe_lst, int i);
int		close_all_pipes(t_shell *shell);
int		wait_all_pid(t_shell *shell);
int		cmd_process(t_shell *shell);	
void	free_split(char **split);
void	print_split(char **split);
int		malloc_pipe_fd(t_shell *shell);
void	close_unused_pipes(t_shell *shell, int i);

#endif
