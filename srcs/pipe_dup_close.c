/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_dup_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:45:22 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/13 11:08:25 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* pipe_fd contains one more slot than needed */

// int	malloc_pipe_fd(t_shell *shell)
// {
// 	int	i;

// 	i = 0;
// 	shell->pipe_fd = malloc(sizeof(int *) * (shell->cmd_nbr - 1));
// 	if (!(shell->pipe_fd))
// 		return (-1);
// 	while (i < shell->cmd_nbr - 1)
// 	{
// 		shell->pipe_fd[i] = malloc(sizeof(int) * 2);
// 		if (!(shell->pipe_fd[i]))
// 			return (-1);
// 		i++;
// 	}
// 	return (0);
// }
