/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/26 12:12:40 by vchevill         ###   ########.fr       */
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


typedef struct s_list_pipes
{
	pid_t						pid;
	char						**command;
	char						*cmd_path;
	int							to_execute;
	int							cmd_index;
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
	char				*readline;
	int					cmd_nbr;
	int					return_val;
	int					is_exit;
	int					save_stdin;
	int					save_stdout;
	int					tmp_stdin;
	int					tmp_stdout;
	char				**envp;
	struct s_list_pipes	*list_start;
	char				*cmd_tmp;
}	t_shell;

/* SIGNALS */

void			sig_int(int code);
void			sig_quit(int code);
void			sig_init(void);
void			eval_child_status(int child_status);
void			ft_print_shell_struct(t_shell	shell);

/* BUILT-IN */

int				print_working_directory(void);
char			*return_working_directory(void);
int				change_directory(t_shell *shell, char *arg);
int				ft_parsing(char *line, t_shell	*shell);
int				ft_free(char *message, t_shell *shell,
					int return_val, int is_exit);
int				ft_echo(t_shell *shell);
int				ft_exit(t_shell *shell, t_list_pipes *pipe);
int				ft_env(t_shell *shell, char *command);
int				ft_export(t_shell *shell, char *s);
int				ft_export_multi(t_shell *shell, char **command);
void			print_sorted_env(t_shell *shell);
int				ft_unset(t_shell *shell, char *s);
int				ft_unset_multi(t_shell *shell, char **command);
char			*ft_getenv(t_shell *shell, char *var_name);
int				is_value_in_envp(t_shell *shell, char *s);
int				get_env_size(t_shell *shell);
void			sort_env(t_shell *shell);

/* PARSING */

int				ft_parse_quotes(int i, int index_start,
					char quote_type, t_shell *shell);
int				ft_new_pipe_chevron(t_shell	*shell);
int				ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell);
void			ft_variable_replace(int i, t_shell *shell);
int				ft_dodge_quotes(char *line, int i);

/* PIPE */

void			concatenate_path(t_shell *shell, t_list_pipes \
	*pipe_lst, char *path);
void			error_cmd_not_found(t_list_pipes *pipe_lst,
					char **cmd);
int				execute_if_built_in(t_shell *shell, t_list_pipes *pipe_lst);
void			cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst);
int				manage_file_fd(t_list_pipes *pipe_lst);
int				manage_dup_fd(t_shell *shell, t_list_pipes *pipe_lst, int i);
int				close_all_pipes(t_shell *shell);
int				wait_all_pid(t_shell *shell);
void			free_split(char **split);
void			print_split(char **split);
int				malloc_pipe_fd(t_shell *shell);
void			close_unused_pipes(t_shell *shell,
					t_list_pipes *pipe_lst, int i);
int				manage_all_file_fd(t_shell *shell);
void			free_fd_tab(t_shell *shell);
int				close_file_pipes(t_shell *shell);
void			ft_free_cmd(t_shell *shell);
int				here_doc(t_shell *shell, t_list_pipes *pipe_lst);
int				here_doc_v2(t_shell *shell, t_list_pipes *pipe_lst);
int				open_in_out_all(t_shell *shell);
void			open_in_out(t_shell *shell, t_list_pipes *pipe_lst);
void			init_fd(t_shell *shell);
void			restore_fd(t_shell *shell);
int				fd_redirect(t_shell *shell, t_list_pipes *pipe_lst, \
	int fd_prev_pipe, int pipe_fd[2]);
void			redirect_file_in_out(t_list_pipes *pipe_lst);

/*EXECUTION*/

void			new_cmd_process(t_shell *shell);
int				execute_if_built_in(t_shell *shell, t_list_pipes *pipe_lst);

/*UTILS*/

void			ft_lstadd_back_pipes(t_list_pipes **alst,
					t_list_pipes *new_elem);
t_list_pipes	*ft_lstnew_pipes(void);
char			**ft_split_quotes(char c, t_shell *shell, int i, int j);
int				ft_strisnum(const char *str);

/*DEBUG*/

void			display_split(char *message, char **split);

extern int		return_val;
#endif
