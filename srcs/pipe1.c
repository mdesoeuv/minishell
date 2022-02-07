/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/04 15:25:19 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* waits for all childs to be terminated */

int	executed_cmd_nbr(t_shell *shell)
{
	t_list_pipes	*pipe_tmp;
	int				counter;

	counter = 0;
	pipe_tmp = shell->list_start;
	while (shell->list_start)
	{
		if (shell->list_start->to_ex == 1)
			counter++;
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = pipe_tmp;
	return (counter);
}

int	wait_all_pid_v2(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_tmp;
	int				child_status;
	int				exec_cmd_nbr;
	int				wait_status;

	exec_cmd_nbr = executed_cmd_nbr(shell);
	i = 0;
	pipe_tmp = shell->list_start;
	while (shell->list_start->next)
	{
		shell->list_start = shell->list_start->next;
	}
	if (shell->list_start->to_ex == 1 && (shell->list_start->is_builtin \
		== 0 || (shell->list_start->is_builtin == 1 && shell->cmd_nbr > 1)))
	{
		waitpid(shell->list_start->pid, &child_status, 0);
		eval_child_status(child_status);
	}
	wait_status = wait(NULL);
	while (wait_status != -1)
		wait_status = wait(NULL);
	shell->list_start = pipe_tmp;
	return (0);
}

int	wait_all_pid(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_tmp;
	int				child_status;

	i = 0;
	pipe_tmp = shell->list_start;
	while (shell->list_start->next)
	{
		if (shell->list_start->to_ex == 1)
			waitpid(shell->list_start->pid, NULL, 0);
		shell->list_start = shell->list_start->next;
		i++;
	}
	if (shell->list_start->to_ex == 1 && (shell->list_start->is_builtin \
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
		if (pipe_lst->chevron_nbr_in > 1)
			pipe_lst->fd_file_in = here_doc_v2(shell, pipe_lst);
		if (pipe_lst->to_ex == 1)
		{
			if (pipe_lst->chevron_nbr_in == 1)
				pipe_lst->fd_file_in = open(pipe_lst->file_in, O_RDONLY);
		}
	}
	if (pipe_lst->file_out != NULL)
	{
		if (pipe_lst->to_ex == 1)
		{
			if (pipe_lst->chevron_nbr_out == 1)
				pipe_lst->fd_file_out = open(pipe_lst->file_out, \
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				pipe_lst->fd_file_out = open(pipe_lst->file_out, \
					O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
	}
	if (pipe_lst->to_ex == 0)
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
