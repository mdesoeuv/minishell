/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 11:45:12 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/05 17:10:35 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*tab;
	size_t	i;

	tab = (char *)malloc(count * size);
	if (!tab)
		return (NULL);
	i = 0;
	while (i < (count * size))
	{
		tab[i] = 0;
		i++;
	}
	return ((void *)tab);
}
