/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_dup_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:45:22 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/11 11:48:38 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_unused_pipes(t_shell *shell, int i)
{
	if (i > 0 && i < shell->pipes_nbr - 1)
		close(shell->pipe_fd[i - 1][1]);
	if (i < shell->pipes_nbr - 1)
		close(shell->pipe_fd[i][0]);
	dprintf(1, "there\n");
}

/* pipe_fd contains one more slot than needed */

int	malloc_pipe_fd(t_shell *shell)
{
	int	i;

	i = 0;
	shell->pipe_fd = malloc(sizeof(int *) * (shell->pipes_nbr));
	if (!(shell->pipe_fd))
		return (-1);
	while (i < shell->pipes_nbr - 1)
	{
		shell->pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!(shell->pipe_fd[i]))
			return (-1);
		i++;
	}
	return (0);
}
