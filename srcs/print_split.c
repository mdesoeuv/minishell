/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:33:32 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/24 11:35:16 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		ft_putendl(split[i]);
		i++;
	}
	ft_putendl("");
}
