/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/28 15:36:29 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell)
{
	char	**cmd_tab;

	cmd_tab = ft_split_quotes(' ', shell);
	if (!cmd_tab)
		return (ft_free("Error : malloc error\n", shell, 1, 1));
	new_pipe->command = cmd_tab;
	return (0);
}

int	ft_dodge_quotes(char *line, int i)
{
	if (line[i] == '\"')
	{
		i++;
		while (line[i] && line[i] != '\"')
			i++;
	}
	else if (line[i] == '\'')
	{
		i++;
		while (line[i] && line[i] != '\'')
			i++;
	}
	if (!line[i])
		return (-1);
	return (i);
}

void	ft_create_file(int chevron_nbr_out, char *file_name)
{
	int	fd;

	if (chevron_nbr_out == 1)
		fd = open(file_name, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(file_name, \
			O_WRONLY | O_APPEND | O_CREAT, 0644);
	close(fd);
}

static void	ft_init_parsing(t_shell	*shell)
{
	shell->list_start = NULL;
	shell->list_start = NULL;
	shell->cmd_tmp = NULL;
	shell->cmd_nbr = 1;
}

/* ft_parsing parse les quotes et découpe en pipes*/

int	ft_parsing(char *line, t_shell	*shell, int i, int start)
{
	ft_init_parsing(shell);
	while (line[++i])
	{
		i = ft_dodge_quotes(line, i);
		if (i == -1)
			return (ft_free(SYNTAX_ERROR, shell, 1, 0));
		if (line[i] == '|')
		{
			shell->cmd_nbr++;
			shell->cmd_tmp = ft_substr(line, start, i - start);
			if (!shell->cmd_tmp)
				return (ft_free("Error : malloc error\n", shell, 1, 0));
			if (ft_new_pipe_chevron(shell) == -1)
				return (-1);
			start = i + 1;
			if (shell->cmd_tmp)
			{
				free(shell->cmd_tmp);
				shell->cmd_tmp = NULL;
			}
		}
	}
	shell->cmd_tmp = ft_substr(line, start, i - start + 1);
	if (ft_new_pipe_chevron(shell) == -1)
		return (-1);
	return (0);
}
