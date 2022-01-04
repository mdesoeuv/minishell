/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 11:38:49 by mdesoeuv          #+#    #+#             */
/*   Updated: 2021/11/03 13:26:19 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isitcomplete(const char *haystack, \
	const char *needle, size_t len)
{
	size_t	i;

	i = 0;
	while (haystack[i] && needle[i])
	{
		if (haystack[i] != needle[i])
			return (0);
		i++;
	}
	if (i <= len)
		return (1);
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!needle || !haystack || needle[0] == 0)
		return ((char *)haystack);
	if (haystack[0] == 0)
		return (NULL);
	if (len == 0 || len < (size_t)ft_strlen(needle))
		return (NULL);
	while (haystack[i] && i < len)
	{
		if (haystack[i] == needle[0])
			if (ft_isitcomplete(&haystack[i], needle, len - i) == 1)
				return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}
