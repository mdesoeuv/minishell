/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/13 11:07:02 by mdesoeuv         ###   ########lyon.fr   */
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

typedef struct s_sig
{
	int				sigint;
	int				exit_status;
	pid_t			pid;
}				t_sig;

/* SIGNALS */

void	sig_int(int code);
void	sig_quit(int code);
void	sig_init(void);
void	ft_print_shell_struct(t_shell	shell);

/* BUILT-IN */

void	print_working_directory(void);
char	*return_working_directory(void);
int		change_directory(char *arg);
void	ft_parsing(char *line, t_shell	*shell);
void	ft_free(char *message, t_shell	*shell, int is_error);
void	ft_echo(t_shell *shell);
int		ft_exit(t_shell *shell);

/* PARSING */

int		ft_parse_quotes(int i, int index_start,
			char quote_type, t_shell *shell);
void	ft_new_pipe_chevron1(t_shell	*shell, int i);
void	ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell);
void	ft_variable_replace(int i, t_shell *shell);

/* PIPE */

void	concatenate_path(t_list_pipes *pipe_lst, char *path);
void	error_cmd_not_found(char **cmd);
void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst);
int		manage_file_fd(t_list_pipes *pipe_lst);
int		manage_dup_fd(t_shell *shell, t_list_pipes *pipe_lst, int i);
int		close_all_pipes(t_shell *shell);
int		wait_all_pid(t_shell *shell);
int		cmd_process(t_shell *shell);	
void	free_split(char **split);
void	print_split(char **split);
int		malloc_pipe_fd(t_shell *shell);
void	close_unused_pipes(t_shell *shell, t_list_pipes *pipe_lst, int i);

extern t_sig	g_sig;
#endif
