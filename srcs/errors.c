/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:59:03 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/28 17:34:20 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_error_msg(char *message, char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(message, 2);
	return (-1);
}

void	no_such_file_error(t_list_pipes *pipe_lst)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(pipe_lst->command[0], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	pipe_lst->to_execute = 0;
	g_return_val = 127;
}

void	error_cmd_not_found(t_list_pipes *pipe_lst, char **cmd)
{
	pipe_lst->to_execute = 0;
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	if (!(pipe_lst->command[0][0] == '.' || pipe_lst->command[0][0] == '/'))
	{
		free(pipe_lst->cmd_path);
		pipe_lst->cmd_path = NULL;
	}
	else
		pipe_lst->cmd_path = NULL;
	g_return_val = 127;
}

int	not_valid_varname(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (ft_is_charset(s[i], "\\;.+=}{*#@!^~/") == 1)
			return (1);
		i++;
	}
	return (0);
}
