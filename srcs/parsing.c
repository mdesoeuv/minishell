/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/17 12:28:27 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell)
{
	char	**cmd_tab;

	cmd_tab = ft_split_quotes(' ', shell, 0, 0);
	if (!cmd_tab)
		return (ft_free("Error : malloc error\n", shell, 1, 1));
	new_pipe->command = cmd_tab;
	return (0);
}

static int	ft_parse_quotes_unclosed(int i, char quote_type, t_shell *shell)
{
	ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
		ft_strlen(shell->cmd_tmp) - i);
	if (!shell->cmd_tmp[i + 1])
		return (ft_free("Error : unclosed quote\n", shell, 1, 0));
	while (shell->cmd_tmp[++i])
	{
		if (shell->cmd_tmp[i] == quote_type)
		{
			ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
				ft_strlen(shell->cmd_tmp) - i);
			break ;
		}
		else if (!shell->cmd_tmp[i + 1])
			return (ft_free("Error : unclosed quote\n", shell, 1, 0));
	}
	return (i);
}

int	ft_parse_quotes(int i, int index_start,
	char quote_type, t_shell *shell)
{
	int	start_quote_index;

	if (i == index_start || shell->cmd_tmp[i - 1] == ' ')
	{
		start_quote_index = i;
		while (shell->cmd_tmp[++i])
		{
			if (quote_type == '\"' && shell->cmd_tmp[i] == '$')
				ft_variable_replace(i, shell);
			if (shell->cmd_tmp[i] == quote_type)
			{
				ft_memmove(&(shell->cmd_tmp[start_quote_index]),
					&(shell->cmd_tmp[start_quote_index + 1]),
					ft_strlen(shell->cmd_tmp) - start_quote_index);
				i--;
				ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
					ft_strlen(shell->cmd_tmp) - i);
				i--;
				break ;
			}
		}
	}
	else if (shell->cmd_tmp[i - 1] != ' ')
		i = ft_parse_quotes_unclosed(i, quote_type, shell);
	return (i);
}

static int	ft_parsing_subfct(char *line, int i)
{
	int	start_i;

	start_i = i;
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
	if (start_i != i)
		i *= -1;
	return (i);
}

/* ft_parsing parse les quotes et découpe en pipes*/
int	ft_parsing(char *line, t_shell	*shell)
{
	int				i;
	int				start;
	int				chevron_start_i;

	i = -1;
	start = 0;
	shell->list_start = NULL;
	shell->cmd_nbr = 1;
	chevron_start_i = -1;
	while (line[++i])
	{
		i = ft_parsing_subfct(line, i);
		if (i < 0)
		{
			i *= -1;
			chevron_start_i = i - start;
			chevron_start_i--;
		}
		if (line[i] == '|')
		{
			shell->cmd_nbr++;
			shell->cmd_tmp = ft_substr(line, start, i - start);
			if (!shell->cmd_tmp)
				return (ft_free("Error : malloc error\n", shell, 1, 0));
			if (ft_new_pipe_chevron1(shell, chevron_start_i) == -1)
				return (-1);
			start = i + 1;
			chevron_start_i = -1;
		}
	}
	shell->cmd_tmp = ft_substr(line, start, i - start + 1);
	if (ft_new_pipe_chevron1(shell, chevron_start_i) == -1)
		return (-1);
	return (0);
}
