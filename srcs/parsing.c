/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/07 12:27:34 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_new_pipe(char *command, t_list_pipes	*pipes_list)
{
	int				i;
	t_list_pipes	pipes_list;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'')
			while (line[i] == '\'')
				if (line[i] == '\'')
					break ;
		if (line[i] == '\"')
			while (line[i] == '\"')
				if (line[i] == '\'')
					break ;
		if (line[i] == '|')
		{
			// new pipe
		}
	}
	// new pipe
}

void	ft_parsing(char *line, t_shell	*shell)
{
	int				i;
	int				start;
	t_list_pipes	*pipes_list;

	i = -1;
	start = 0;
	pipes_list = NULL;
	shell->list_start = &pipes_list;
	while (line[++i])
	{
		if (line[i] == '\'')
			while (line[++i] == '\'')
				if (line[i] == '\'')
					break ;
		if (line[i] == '\"')
			while (line[++i] == '\"')
				if (line[i] == '\'')
					break ;
		if (line[i] == '|')
		{
			ft_new_pipe(ft_substr(line, i - start, i), &pipes_list);
			start = i;
		}
	}
	ft_new_pipe(ft_substr(line, i - start, i), &pipes_list);
}
