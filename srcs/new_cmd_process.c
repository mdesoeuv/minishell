/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_cmd_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:32:46 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/21 16:57:34 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fd(t_shell *shell)
{
	// malloc_pipe_fd(shell);
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

int	fd_redirect(t_shell *shell, t_list_pipes *pipe_lst, \
	int fd_prev_pipe, int pipe_fd[2])
{
	int	fd_read;

	fd_read = dup(pipe_fd[0]);
	if (pipe_lst->cmd_index > 0)
	{
		dup2(fd_prev_pipe, 0);
		close(fd_prev_pipe);
	}
	if (pipe_lst->next)
	{
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		dup2(shell->save_stdout, 1);
	}
	return (fd_read);
}

int	open_in_out(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in)
	{
		if (pipe_lst->chevron_nbr_in == 1)
			pipe_lst->fd_file_in = \
			open(pipe_lst->file_in, O_RDONLY);
		else if (pipe_lst->chevron_nbr_in > 1)
		{
			here_doc(shell, pipe_lst);
			pipe_lst->fd_file_in = open("tmp/heredoc", O_RDONLY);
			dprintf(1, "file fd = %d\n", pipe_lst->fd_file_in);
		}
		close(0);
		dup2(pipe_lst->fd_file_in, 0);
		close(pipe_lst->fd_file_in);
	}
	if (pipe_lst->file_out != NULL)
	{
		if (pipe_lst->chevron_nbr_out == 1)
			pipe_lst->fd_file_out = open(pipe_lst->file_out, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			pipe_lst->fd_file_out = open(pipe_lst->file_out, \
				O_WRONLY | O_APPEND | O_CREAT, 0644);
		close(1);
		dup2(pipe_lst->fd_file_out, 1);
		close(pipe_lst->fd_file_out);
	}
	if (pipe_lst->fd_file_in < 0)
		pipe_lst->to_execute = 0;
	return (0);
}

void	execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	pipe_lst->pid = fork();
	if (pipe_lst->pid < 0)
	{
		ft_free("minishell: fork error\n", shell, 1, 1);
	}
	else if (pipe_lst->pid == 0)
	{
		cmd_test_execute(shell, pipe_lst);
		ft_free("", shell, 1, 1);
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
	init_fd(shell);
	fd_prev_pipe = 0;
	while (shell->list_start)
	{
		shell->list_start->cmd_index = i++;
		g_sig.pid = 1;
		if (pipe(pipe_fd) == -1)
			ft_free("minishell: pipe error\n", shell, 1, 1);
		fd_prev_pipe = fd_redirect(shell, shell->list_start, \
			fd_prev_pipe, pipe_fd);
		open_in_out(shell, shell->list_start);
		if (shell->list_start->to_execute == 1)
		{
			if (execute_if_built_in(shell, shell->list_start) == -100)
				execute(shell, shell->list_start);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = start;
	wait_all_pid(shell);
	close_file_pipes(shell);
	restore_fd(shell);
	g_sig.pid = 0;
}
