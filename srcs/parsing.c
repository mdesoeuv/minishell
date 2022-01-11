/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/11 17:04:55 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell)
{
	char	**cmd_tab;

	cmd_tab = ft_split_quotes(' ', shell, 0, 0);
	if (!cmd_tab)
		ft_free("Error : malloc error\n", shell, 1);
	new_pipe->command = cmd_tab;
}

static void	ft_parse_quotes_unclosed(int i, char quote_type, t_shell *shell)
{
	ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
		ft_strlen(shell->cmd_tmp) - i);
	if (!shell->cmd_tmp[i + 1])
	{
		ft_free("Error : unclosed quote\n", shell, -1);
		return ;
	}
	while (shell->cmd_tmp[++i])
	{
		if (shell->cmd_tmp[i] == quote_type)
		{
			ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
				ft_strlen(shell->cmd_tmp) - i);
			break ;
		}
		else if (!shell->cmd_tmp[i + 1])
		{
			ft_free("Error : unclosed quote\n", shell, -1);
			return ;
		}
	}
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
		ft_parse_quotes_unclosed(i, quote_type, shell);
	return (i);
}

/* ft_parsing parse les quotes et découpe en pipes*/
void	ft_parsing(char *line, t_shell	*shell)
{
	int				i;
	int				start;

	i = -1;
	start = 0;
	shell->list_start = NULL;
	shell->pipes_nbr = 1;
	while (line[++i])
	{
		if (line[i] == '|')
		{
			shell->pipes_nbr++;
			shell->cmd_tmp = ft_substr(line, start, i - start);
			ft_new_pipe_chevron1(shell, -1);
			start = i + 1;
		}
	}
	shell->cmd_tmp = ft_substr(line, start, i - start + 1);
	ft_new_pipe_chevron1(shell, -1);
}
