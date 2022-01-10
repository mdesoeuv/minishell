/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 14:45:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/09 15:31:23 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_shell *shell)
{
	int	return_num;

	ft_putstr_fd("exit", 1);
	return_num = 0;
	if (shell->list_start->command[1]
		&& ft_strisnum(shell->list_start->command[1]) == 0)
	{
		ft_putstr_fd("\nminishell: exit: ", 1);
		ft_putstr_fd(shell->list_start->command[1], 1);
		ft_putendl_fd(" : numeric argument required", 1);
		return_num = 1;
	}
	else if (shell->list_start->command[1] && shell->list_start->command[2])
	{
		return_num = 1;
		ft_putendl_fd("\nminishell: exit: too many arguments", 1);
	}
	else if (shell->list_start->command[1])
		ft_free("", shell, ft_atoi(shell->list_start->command[1]));
	else
		ft_free("", shell, 0);
	return (return_num);
}
