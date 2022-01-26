/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 14:45:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/26 12:00:00 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_shell *shell, t_list_pipes *pipe)
{
	int	return_num;

	g_sig.sigint = 1;
	if (shell->cmd_nbr < 1)
		ft_putstr_fd("exit\n", 1);
	return_num = 0;
	if (pipe->command[1]
		&& ft_strisnum(pipe->command[1]) == 0)
	{
		ft_putstr_fd("minishell: exit: ", 1);
		ft_putstr_fd(pipe->command[1], 1);
		ft_putendl_fd(" : numeric argument required\n", 1);
		g_sig.exit_status = 255;
		return_num = 1;
	}
	else if (pipe->command[1] && pipe->command[2])
	{
		return_num = 1;
		ft_putendl_fd("minishell: exit: too many arguments\n", 1);
	}
	else if (pipe->command[1])
		ft_free("", shell, ft_atoi(pipe->command[1]), 1);
	else
		ft_free("", shell, 0, 1);
	return (return_num);
}
