/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 11:53:58 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/06 13:03:22 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	building absolute path
*/

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
	int		i;
	int		count;
	int		len;
	char	*trim_path;

	len = ft_strlen(s);
	count = 0;
	i = len - 1;
	while (s[i] && s[i] != '/')
	{
		i--;
		count++;
	}
	if (i < 0)
		return (NULL);
	trim_path = ft_strndup(s, len - count - 1);
	printf("trim_path = %s\n", trim_path);
	return (trim_path);
}

char	*up_directory_build(char *path)
{
	char	*new_path;

	new_path = trim_path(return_working_directory());
	if (!new_path)
		return (NULL);
	new_path = ft_strjoin_free(new_path, offset_dup(path, 2));
	return (new_path);
}

char	*build_path(char *arg)
{
	if (arg[0] != '.')
		return (arg);
	else if (arg[1] != '.')
		return (ft_strjoin_free(return_working_directory(), \
			offset_dup(arg, 1)));
	else
		return (up_directory_build(arg));
}

int	change_directory(char *arg)
{
	char	*path;

	path = build_path(arg);
	if (!path)
		return (-1);
	dprintf(1, "new_path = %s\n", path);
	return (chdir(path));
}
