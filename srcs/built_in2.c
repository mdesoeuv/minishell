/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 11:53:58 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/05 12:23:30 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*offset_dup(char *s, size_t offset)
{
	size_t	len;
	size_t	i;
	char	*path;

	len = ft_strlen(s);
	path = malloc(sizeof(char) * (len - offset + 1));
	if (!path)
		return (NULL);
	i = 0;
	while (offset < len)
		path[i++] = s[offset++];
	path[i] = 0;
	return (path);
}

char	*trim_path(char *s)
{
	int	i;
	int	count;
	int	len;

	len = ft_strlen(s);
	count -
	
}

int	build_path(char *arg)
{
	char	*tmp;
	
	if (arg[0] != '.')
		return (1);
	tmp = arg;
	if (arg[1] != '.')
		
	else
		
	free(tmp);
	return (1);
}

int	change_directory(char *arg)
{
	if (build_path(arg) == -1)
		return (-1);
	chdir(arg);
	return (0);
}
