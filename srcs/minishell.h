/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/01/17 09:45:23 by mdesoeuv         ###   ########lyon.fr   */
=======
/*   Updated: 2022/01/17 12:32:16 by vchevill         ###   ########.fr       */
>>>>>>> f695ea088d45fdc2363487ffd964e11642d7ecd6
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

typedef struct s_list_pipes
{
	pid_t						pid;
	char						**command;
	char						*cmd_path;
	char						*file_in;
	int							fd_file_in;
	int							chevron_nbr_in;
	char						*file_out;
	int							fd_file_out;
	int							chevron_nbr_out;
	struct s_list_pipes			*next;
}	t_list_pipes;

typedef struct s_shell
{
	int					cmd_nbr;
	int					return_val;
	int					is_exit;
	int					**pipe_fd;
	char				**envp;
	struct s_list_pipes	*pipe_lst;
	struct s_list_pipes	*list_start;
	char				*cmd_tmp;
}	t_shell;

/* SIGNALS */

void			sig_int(int code);
void			sig_quit(int code);
void			sig_init(void);
void			ft_print_shell_struct(t_shell	shell);

/* BUILT-IN */

int				print_working_directory(void);
// char			*return_working_directory(void);
int				change_directory(char *arg);
int				ft_parsing(char *line, t_shell	*shell);
int				ft_free(char *message, t_shell *shell,
					int return_val, int is_exit);
void			ft_echo(t_shell *shell);
int				ft_exit(t_shell *shell);
int				ft_env(t_shell *shell, char *command);
int				ft_export(t_shell *shell, char *s);
int				ft_unset(t_shell *shell, char *s);
char			*ft_getenv(t_shell *shell, char *var_name);
int				is_value_in_envp(t_shell *shell, char *s);
int				get_env_size(t_shell *shell);

/* PARSING */

int				ft_parse_quotes(int i, int index_start,
					char quote_type, t_shell *shell);
int				ft_new_pipe_chevron1(t_shell	*shell, int i);
int				ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell);
void			ft_variable_replace(int i, t_shell *shell);

/* PIPE */

void			concatenate_path(t_list_pipes *pipe_lst, char *path);
void			error_cmd_not_found(char **cmd, char **possible_paths);
void			cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst);
int				manage_file_fd(t_list_pipes *pipe_lst);
int				manage_dup_fd(t_shell *shell, t_list_pipes *pipe_lst, int i);
int				close_all_pipes(t_shell *shell);
int				wait_all_pid(t_shell *shell);
int				cmd_process(t_shell *shell);	
void			free_split(char **split);
void			print_split(char **split);
int				malloc_pipe_fd(t_shell *shell);
void			close_unused_pipes(t_shell *shell,
					t_list_pipes *pipe_lst, int i);
int				manage_all_file_fd(t_shell *shell);
void			free_fd_tab(t_shell *shell);
int				close_file_pipes(t_shell *shell);
void			ft_free_cmd(t_shell *shell);

/*UTILS*/

void			ft_lstadd_back_pipes(t_list_pipes **alst,
					t_list_pipes *new_elem);
t_list_pipes	*ft_lstnew_pipes(void);
char			**ft_split_quotes(char c, t_shell *shell, int i, int j);
int				ft_strisnum(const char *str);

/*DEBUG*/

void			display_split(char *message, char **split);

extern t_sig	g_sig;
#endif
