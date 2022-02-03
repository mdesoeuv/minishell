/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 14:45:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/03 10:27:13 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_shell *shell, t_list_pipes *pipe)
{
	int	return_num;

	if (shell->cmd_nbr < 2)
		ft_putstr_fd("exit\n", 2);
	return_num = 0;
	if (pipe->command[1]
		&& ft_strisnum(pipe->command[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(pipe->command[1], 2);
		ft_putstr_fd(" : numeric argument required\n", 2);
		g_return_val = 255;
		ft_free("", shell, g_return_val, 1);
		return_num = 1;
	}
	else if (pipe->command[1] && pipe->command[2])
	{
		return_num = 1;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	}
	else if (pipe->command[1])
		ft_free("", shell, ft_atoi(pipe->command[1]), 1);
	else
		ft_free("", shell, g_return_val, 1);
	return (return_num);
}
