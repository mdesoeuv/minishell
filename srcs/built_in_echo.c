/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 14:29:22 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/25 13:33:00 by vchevill         ###   ########lyon.fr   */
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
	int		j;

	i = 1;
	j = 1;
	option_n = 0;
	if (nb_args(shell->list_start->command) > 1)
	{
		if (shell->list_start->command[i]
			&& ft_strncmp(shell->list_start->command[i], "-n", 2) == 0)
		{
			while (shell->list_start->command[i][j]
				|| shell->list_start->command[i][j] != ' ')
			{
				if (shell->list_start->command[i][j] != 'n')
					break ;
				j++;
			}
			if (!shell->list_start->command[i][j]
				|| shell->list_start->command[i][j] == ' ')
			{
				option_n = 1;
				i++;	
			}
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
