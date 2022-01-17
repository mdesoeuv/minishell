/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_cmd_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/17 16:32:46 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/17 18:14:59 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_fd(t_shell *shell)
{
	malloc_pipe_fd(shell);
	shell->save_fd_in = dup(0);
	shell->save_fd_out = dup(1);
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
		pipe_lst->fd_in = dup(shell->save_fd_in);
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
		pipe_lst->fd_out = dup(shell->save_fd_out);
	if (pipe_lst->fd_in < 0 || pipe_lst->fd_out < 0)
	{
		perror("minishell");
		return (-1);
	}
	return (0);
}

void	execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (execute_if_built_in(shell, pipe_lst) != -100)
		return ;
	pipe_lst->pid = fork();
	if (pipe_lst->pid < 0)
	{
		perror("minishell");
		ft_free("", shell, shell->return_val, 1);
	}
	else if (pipe_lst->pid == 0)
	{
		cmd_test_execute(shell, pipe_lst);
		ft_free("", shell, shell->return_val, 1);
	}
}

void	new_cmd_process(t_shell *shell)
{
	t_list_pipes	*start;
	int				i;
	// int				pipe_fd[2];

	i = 0;
	start = shell->list_start;
	init_fd(shell);
	// manage_all_file_fd(shell);
	while (shell->list_start)
	{
		manage_file_fd(shell, shell->list_start);
		dup2(shell->list_start->fd_in, 0);
		close(shell->list_start->fd_in);
		if (i == shell->cmd_nbr - 1 && !(shell->list_start->file_out))
			shell->list_start->fd_out = dup(shell->save_fd_out);
		if (i != shell->cmd_nbr - 1 && shell->cmd_nbr > 1)
			pipe(shell->pipe_fd[i]);
		if (i != shell->cmd_nbr - 1 && shell->cmd_nbr > 1)
			shell->list_start->fd_out = shell->pipe_fd[i][1];
		if (shell->cmd_nbr > 1 && i != shell->cmd_nbr - 1)
			shell->list_start->fd_in = shell->pipe_fd[i][0];
		dup2(shell->list_start->fd_out, 1);
		close(shell->list_start->fd_out);
		execute(shell, shell->list_start);
		// if (shell->cmd_nbr > 1 && i < shell->cmd_nbr && i > 0)
		// {
		// 	close(shell->pipe_fd[i - 1][0]);
		// 	close(shell->pipe_fd[i - 1][1]);
		// }
		shell->list_start = shell->list_start->next;
		i++;
	}
	dup2(shell->save_fd_in, 0);
	dup2(shell->save_fd_out, 1);
	close(shell->save_fd_in);
	close(shell->save_fd_out);
	shell->list_start = start;
	wait_all_pid(shell);
}
