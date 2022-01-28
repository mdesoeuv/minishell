/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/28 16:14:39 by mdesoeuv         ###   ########lyon.fr   */
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
	if (shell->list_start->to_execute == 1)
		waitpid(shell->list_start->pid, &child_status, 0);
	if (shell->list_start->to_execute == 1 \
		&& shell->list_start->is_builtin == 0)
		eval_child_status(child_status);
	shell->list_start = pipe_tmp;
	return (0);
}

void	open_in_out(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in)
	{
		if (pipe_lst->to_execute == 1)
		{
			if (pipe_lst->chevron_nbr_in == 1)
				pipe_lst->fd_file_in = open(pipe_lst->file_in, O_RDONLY);
			else if (pipe_lst->chevron_nbr_in > 1)
				pipe_lst->fd_file_in = here_doc_v2(shell, pipe_lst);
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


int	ft_check_if_file_exists(t_list_pipes *pipe_lst, char *file_name)
{
	if (access(file_name, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		pipe_lst->to_execute = 0;
		return (0);
	}
	else if (access(file_name, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		pipe_lst->to_execute = 0;
		return (0);
	}
	else if (is_directory(pipe_lst, file_name) == 1)
	{
		pipe_lst->to_execute = 0;
		return (0);
	}
	return (1);
}
