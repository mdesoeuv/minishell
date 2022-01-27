/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:32:46 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/27 13:45:34 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fd(t_shell *shell)
{
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
	close(pipe_fd[0]);
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

void	redirect_file_in_out(t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in)
	{
		close(0);
		dup2(pipe_lst->fd_file_in, 0);
		close(pipe_lst->fd_file_in);
	}
	if (pipe_lst->file_out != NULL)
	{
		close(1);
		dup2(pipe_lst->fd_file_out, 1);
		close(pipe_lst->fd_file_out);
	}
}

int	close_file_pipes(t_shell *shell)
{
	int				i;
	t_list_pipes	*start_lst;

	start_lst = shell->list_start;
	i = 0;
	while (shell->list_start != NULL)
	{
		if (shell->list_start->file_in != NULL)
			close(shell->list_start->fd_file_in);
		if (shell->list_start->file_out != NULL)
			close(shell->list_start->fd_file_out);
		shell->list_start = shell->list_start->next;
		i++;
	}
	shell->list_start = start_lst;
	return (0);
}
