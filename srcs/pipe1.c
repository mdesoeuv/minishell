/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/03 10:14:16 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* waits for all childs to be terminated */

int	wait_all_pid(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_tmp;
	int				child_status;

	i = 0;
	pipe_tmp = shell->list_start;
	while (shell->list_start->next)
	{
		if (shell->list_start->to_execute == 1)
			waitpid(shell->list_start->pid, NULL, 0);
		shell->list_start = shell->list_start->next;
		i++;
	}
	if (shell->list_start->to_execute == 1 && (shell->list_start->is_builtin \
		== 0 || (shell->list_start->is_builtin == 1 && shell->cmd_nbr > 1)))
	{
		waitpid(shell->list_start->pid, &child_status, 0);
		eval_child_status(child_status);
	}
	shell->list_start = pipe_tmp;
	return (0);
}

void	open_in_out(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in)
	{
		// ft_check_if_file_in_exists(pipe_lst);
		if (pipe_lst->chevron_nbr_in > 1)
			pipe_lst->fd_file_in = here_doc_v2(shell, pipe_lst);
		if (pipe_lst->to_execute == 1)
		{
			if (pipe_lst->chevron_nbr_in == 1)
				pipe_lst->fd_file_in = open(pipe_lst->file_in, O_RDONLY);
		}
	}
	if (pipe_lst->file_out != NULL)
	{
		if (pipe_lst->to_execute == 1)
		{
			if (pipe_lst->chevron_nbr_out == 1)
				pipe_lst->fd_file_out = open(pipe_lst->file_out, \
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				pipe_lst->fd_file_out = open(pipe_lst->file_out, \
					O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
	}
	if (pipe_lst->to_execute == 0)
		g_return_val = 1;
}

int	open_in_out_all(t_shell *shell)
{
	t_list_pipes	*start;

	start = shell->list_start;
	while (shell->list_start)
	{
		open_in_out(shell, shell->list_start);
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = start;
	return (0);
}

int	ft_check_if_file_exists(t_list_pipes *pipe, t_shell *shell)
{
	if (shell->no_such_file == 1)
		return (0);
	if (access(pipe->file_in, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(pipe->file_in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->no_such_file = 1;
		return (0);
	}
	if (access(pipe->file_in, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(pipe->file_in, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		shell->no_such_file = 1;
		return (0);
	}
	if (is_directory(pipe, pipe->file_in) == 1)
	{
		shell->no_such_file = 1;
		return (0);
	}
	return (1);
}

int	ft_check_if_file_in_exists(t_list_pipes *pipe)
{
	if (access(pipe->file_in, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(pipe->file_in, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		pipe->to_execute = 0;
		return (0);
	}
	if (access(pipe->file_in, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(pipe->file_in, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		pipe->to_execute = 0;
		return (0);
	}
	if (is_directory(pipe, pipe->file_in) == 1)
	{
		pipe->to_execute = 0;
		return (0);
	}
	return (1);
}
