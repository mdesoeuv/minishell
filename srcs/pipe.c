/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/13 15:35:15 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_all_pid(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_tmp;

	i = 0;
	dprintf(1, "waiting all pid\n");
	pipe_tmp = shell->list_start;
	while (shell->list_start)
	{
		dprintf(1, "waited for %d\n", shell->list_start->pid);
		waitpid(shell->list_start->pid, NULL, 0);
		shell->list_start = shell->list_start->next;
		i++;
	}
	shell->list_start = pipe_tmp;
	dprintf(1, "all pid finished !\n");
	return (0);
}

int	malloc_pipe_fd(t_shell *shell)
{
	int	i;

	i = 0;
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
