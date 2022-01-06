/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 16:43:20 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/06 11:34:56 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	MAX_PATH = 1024 in OSX
*/

void	print_working_directory(void)
{
	char	path[1025];
	char	*result;

	result = getcwd(path, 1024);
	ft_putendl_fd(result, 1);
}

char	*return_working_directory(void)
{
	char	path[1025];

	return (ft_strdup(getcwd(path, 1024)));
}
