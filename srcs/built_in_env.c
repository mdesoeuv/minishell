/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:45:42 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/14 15:03:04 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_shell *shell, char *command)
{
	int	i;

	if (command != NULL)
	{
		ft_putendl_fd("minishell: env: no argument supported", 2);
		return (0);
	}
	i = 0;
	while (shell->envp[i])
	{
		ft_putendl_fd(shell->envp[i], 1);
		i++;
	}
	return (0);
}
