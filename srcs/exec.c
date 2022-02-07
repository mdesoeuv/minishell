/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 11:32:38 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/07 13:09:05 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(t_list_pipes *pipe_lst, char *path)
{
	DIR	*dirptr;

	dirptr = opendir(path);
	if (!dirptr)
		return (0);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	pipe_lst->to_ex = 0;
	closedir(dirptr);
	g_return_val = 126;
	return (1);
}

void	execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (ft_getenv(shell, "PATH") == NULL
		&& !(pipe_lst->command[0][0] == '.' || pipe_lst->command[0][0] == '/'))
		no_such_file_error(pipe_lst);
	else if (pipe_lst->command[0][0] == '.' || pipe_lst->command[0][0] == '/')
	{
		if (access(pipe_lst->command[0], F_OK) == -1)
		{
			no_such_file_error(pipe_lst);
			return ;
		}
		else
		{
			if (is_directory(pipe_lst, pipe_lst->command[0]) == 1)
				return ;
		}
	}
	pipe_lst->pid = fork();
	if (pipe_lst->pid < 0)
		ft_fork_error(shell);
	else if (pipe_lst->pid == 0)
	{
		if (pipe_lst->to_ex == 1)
			cmd_test_execute(shell, pipe_lst);
		ft_free("", shell, g_return_val, 1);
	}
}

static void	end_cmd_process(t_shell *shell)
{
	wait_all_pid_v2(shell);
	close_file_pipes(shell);
	restore_fd(shell);
	shell->fork_error = 0;
}

static void	cmd_middle_process(t_shell *shell, int pipe_fd[2], \
	int *fd_prev_pipe, int *i)
{
	shell->list_start->cmd_index = *i;
	if (pipe(pipe_fd) == -1)
		ft_free("minishell: pipe error\n", shell, g_return_val, 1);
	*fd_prev_pipe = fd_redirect(shell, shell->list_start, \
		*fd_prev_pipe, pipe_fd);
	redirect_file_in_out(shell->list_start);
	if (shell->list_start->to_ex == 1)
	{
		if (execute_if_built_in(shell, shell->list_start) == -100)
			execute(shell, shell->list_start);
	}
	close(1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	*i += 1;
}

void	new_cmd_process(t_shell *shell)
{
	t_list_pipes	*start;
	int				i;
	int				pipe_fd[2];
	int				fd_prev_pipe;

	i = 0;
	start = shell->list_start;
	init_fd(shell);
	open_in_out_all(shell);
	fd_prev_pipe = 0;
	signal(SIGINT, sig_int_fork);
	while (shell->list_start && shell->fork_error == 0)
	{
		cmd_middle_process(shell, pipe_fd, &fd_prev_pipe, &i);
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = start;
	close(fd_prev_pipe);
	end_cmd_process(shell);
}
