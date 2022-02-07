/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 14:45:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/07 11:37:04 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_overflow(char *nbr)
{
	long int	n;

	if (!nbr)
		return (0);
	n = ft_atol(nbr);
	if ((n == -1 && ft_strcmp(nbr, "-1") != 0) || \
		((n == 0 && ft_strcmp(nbr, "0") != 0) && (n == 0 && \
		ft_strcmp(nbr, "-0") != 0) && (n == 0 && ft_strcmp(nbr, "+0") != 0)))
		return (1);
	return (0);
}

int	ft_exit(t_shell *shell, t_list_pipes *pipe)
{
	int	return_num;

	if (shell->cmd_nbr < 2)
		ft_putstr_fd("exit\n", 2);
	return_num = 0;
	if ((pipe->command[1] && ft_strisnum(pipe->command[1]) \
		== 0) || is_overflow(pipe->command[1]) == 1)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(pipe->command[1], 2);
		ft_putstr_fd(" : numeric argument required\n", 2);
		ft_free("", shell, 255, 1);
		return_num = 1;
	}
	else if (pipe->command[1] && pipe->command[2])
	{
		return_num = 1;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	}
	else if (pipe->command[1])
		ft_free("", shell, ft_atol(pipe->command[1]) % 256, 1);
	else
		ft_free("", shell, g_return_val, 1);
	return (return_num);
}
