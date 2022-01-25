/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 16:43:20 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/25 14:29:04 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	MAX_PATH = 1024 in OSX => To replace with getcwd()?
*/

int	print_working_directory(void)
{
	char	path[PATH_MAX];
	char	*result;

	result = getcwd(path, PATH_MAX);
	ft_putendl_fd(result, 1);
	return (0);
}

char	*return_working_directory(void)
{
	char	path[PATH_MAX];

	return (ft_strdup(getcwd(path, PATH_MAX)));
}
