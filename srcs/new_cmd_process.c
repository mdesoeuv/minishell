/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_cmd_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:32:46 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/19 21:37:11 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fd(t_shell *shell)
{
	malloc_pipe_fd(shell);
	shell->save_stdin = dup(0);
	shell->save_stdout = dup(1);
}

void	restore_fd(t_shell *shell)
{
	dup2(shell->save_stdin, 0);
	close(shell->save_stdin);
	dup2(shell->save_stdout, 1);
	close(shell->save_stdout);
}

void	fd_child(t_list_pipes *pipe_lst, int pipe_fd[2], int *fd_prev_pipe)
{
	close(pipe_fd[0]);
	if (fd_prev_pipe != -1)
	{
		close(0);
		dup(*fd_prev_pipe);
		close(*fd_prev_pipe);
	}
	if (pipe_lst->next)
	{
		close(1);
		dup(pipe_fd[1]);
		close(pipe_fd[1]);
	}
}

int	open_in_out(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in)
	{
		if (pipe_lst->chevron_nbr_in == 1)
			pipe_lst->fd_in = \
			open(pipe_lst->file_in, O_RDONLY);
		else if (pipe_lst->chevron_nbr_in > 1)
		{
			ft_putstr("<< not yet managed\n");
			pipe_lst->fd_file_in = 0;
		}
	}
	else
		pipe_lst->fd_in = dup(shell->save_stdin);
	if (pipe_lst->file_out != NULL)
	{
		if (pipe_lst->chevron_nbr_out == 1)
			pipe_lst->fd_out = open(pipe_lst->file_out, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			pipe_lst->fd_out = open(pipe_lst->file_out, \
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	}
	else
		pipe_lst->fd_out = dup(shell->save_stdout);
	if (pipe_lst->fd_in < 0 || pipe_lst->fd_out < 0)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}

void	execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	// if (execute_if_built_in(shell, pipe_lst) != -100)
	// 	return ;
	pipe_lst->pid = fork();
	if (pipe_lst->pid < 0)
	{
		perror("minishell");
		ft_free("", shell, shell->return_val, 1);
	}
	else if (pipe_lst->pid == 0)
	{
		cmd_test_execute(shell, pipe_lst);
		ft_free("minishell: execve error\n", shell, 1, 1);
	}
}

void	new_cmd_process(t_shell *shell)
{
	t_list_pipes	*start;
	int				i;
	int				pipe_fd[2];
	int				fd_prev_pipe;

	i = 0;
	start = shell->list_start;
	pipe_fd[0] = -1;
	init_fd(shell);
	fd_prev_pipe = 0;
	while (shell->list_start)
	{
		fd_prev_pipe = pipe_fd[0];
		if (pipe(pipe_fd) == -1)
			ft_free("minishell: pipe error\n", shell, 1, 1);
		fd_child(shell->list_start, pipe_fd, &fd_prev_pipe);
		if (execute_if_built_in(shell, shell->list_start) == -100)
			execute(shell, shell->list_start);
		close(fd_prev_pipe);
		close(pipe_fd[1]);
		shell->list_start = shell->list_start->next;
		i++;
	}
	shell->list_start = start;
	wait_all_pid(shell);
	restore_fd(shell);
}
