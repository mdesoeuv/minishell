/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 14:29:22 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/17 09:45:00 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	nb_args(char **args)
{
	int		size;

	size = 0;
	while (args[size])
		size++;
	return (size);
}

int	ft_echo(t_shell *shell)
{
	int		i;
	int		option_n;

	i = 1;
	option_n = 0;
	if (nb_args(shell->list_start->command) > 1)
	{
		while (shell->list_start->command[i]
			&& ft_strcmp(shell->list_start->command[i], "-n") == 0)
		{
			option_n = 1;
			i++;
		}
		while (shell->list_start->command[i])
		{
			ft_putstr(shell->list_start->command[i]);
			if (shell->list_start->command[i + 1]
				&& shell->list_start->command[i][0] != '\0')
				ft_putchar(' ');
			i++;
		}
	}
	if (option_n == 0)
		ft_putchar('\n');
	return (0);
}
