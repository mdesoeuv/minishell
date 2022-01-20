/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/20 19:33:55 by mdesoeuv         ###   ########lyon.fr   */
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
		waitpid(shell->list_start->pid, NULL, 0);
		shell->list_start = shell->list_start->next;
		i++;
	}
	waitpid(shell->list_start->pid, &child_status, 0);
	shell->list_start = pipe_tmp;
	eval_child_status(child_status);
	return (0);
}

int	malloc_pipe_fd(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->cmd_nbr <= 1)
	{
		shell->pipe_fd = NULL;
		return (0);
	}
	shell->pipe_fd = malloc(sizeof(int *) * (shell->cmd_nbr - 1));
	if (!(shell->pipe_fd))
		return (-1);
	while (i < shell->cmd_nbr - 1)
	{
		shell->pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!(shell->pipe_fd[i]))
			return (-1);
		i++;
	}
	return (0);
}

void	free_fd_tab(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->cmd_nbr - 1)
	{
		free(shell->pipe_fd[i]);
		i++;
	}
	free(shell->pipe_fd);
}
