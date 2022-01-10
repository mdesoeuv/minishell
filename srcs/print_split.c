/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:33:32 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/10 17:42:14 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_split(char **split)
{
	int	i;

	i = 0;
	dprintf(1, "split =\n");
	while (split[i])
	{
		ft_putendl(split[i]);
		i++;
	}
	ft_putendl("");
}
