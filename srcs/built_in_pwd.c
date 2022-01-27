/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 16:43:20 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/27 10:45:17 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	MAX_PATH = 1024 in OSX
*/

int	print_working_directory(void)
{
	char	path[PATH_MAX + 1];
	char	*result;

	result = getcwd(path, PATH_MAX);
	result[PATH_MAX] = 0;
	ft_putendl_fd(result, 1);
	return (0);
}

char	*return_working_directory(void)
{
	char	path[PATH_MAX + 1];
	char	*result;

	result = getcwd(path, PATH_MAX);
	result[PATH_MAX] = 0;
	return (ft_strdup(result));
}
