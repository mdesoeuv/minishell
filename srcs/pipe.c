/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/07 16:02:59 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cmd_process(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < shell->pipe_nbr + 1)
	{
		pipe(shell->pipe_fd[i]);
		shell->pid[i] = fork();
		if (shell->pid[i] < 0)
		{
			perror("minishell");
			exit(EXIT_FAILURE);
		}
		else if (shell->pid[i] == 0)
		{
			test_execute_cmd(shell->cmd_list[i]);
			return (0);
		}
		else
			i++;
	}
	close_all_pipes(shell);
	wait_all_pid(shell);
}
