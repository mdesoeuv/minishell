/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 16:43:20 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/04 15:10:55 by vchevill         ###   ########lyon.fr   */
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

char	*return_working_directory(char *freeval, t_shell *shell)
{
	char	path[PATH_MAX + 1];
	char	*result;
	char	*ret;

	result = getcwd(path, PATH_MAX);
	result[PATH_MAX] = 0;
	ret = ft_strdup(result);
	if (!ret)
	{
		free(freeval);
		ft_free("memory allocation error\n", shell, 1, 1);
	}
	return (ret);
}
