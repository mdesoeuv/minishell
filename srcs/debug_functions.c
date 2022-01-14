/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 10:49:14 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/14 10:52:02 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	display_split(char *message, char **split)
{
	int	i;

	ft_putstr("Displaying ");
	ft_putendl(message);
	i = 0;
	while (split[i])
	{
		ft_putendl(split[i]);
		i++;
	}
	ft_putendl("--end");
}
