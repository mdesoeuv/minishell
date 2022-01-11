/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/11 16:33:26 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



void	ft_variable_replace(int i, t_shell *shell)
{
	int		index_start;
	char	*variable_name;
	char	*variable_result;
	char	*tmp;

	index_start = i + 1;
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != ' ' && shell->cmd_tmp[i] != '\"')
		i++;
	variable_name = ft_substr(shell->cmd_tmp, index_start, i - index_start);
	if (!variable_name)
		ft_free("Error : malloc error\n", shell, 1);
	variable_result = getenv(variable_name);
	free(variable_name);
	if (variable_result)
	{
		ft_memmove(&(shell->cmd_tmp[index_start - 1]), &(shell->cmd_tmp[i]),
			ft_strlen(shell->cmd_tmp) - index_start - 1);
		tmp = ft_strndup(shell->cmd_tmp, index_start - 1);
		if (!tmp)
			ft_free("Error : malloc error\n", shell, 1);
		tmp = ft_strjoin(tmp, variable_result);
		if (!shell->cmd_tmp)
			ft_free("Error : malloc error\n", shell, 1);
		shell->cmd_tmp = ft_strjoin(tmp, ft_substr(shell->cmd_tmp, index_start - 1, ft_strlen(shell->cmd_tmp) - index_start + 1));
		if (!shell->cmd_tmp)
			ft_free("Error : malloc error\n", shell, 1);
		free(tmp);
		return ;
	}
	ft_memmove(&(shell->cmd_tmp[index_start - 1]), &(shell->cmd_tmp[i]),
		ft_strlen(shell->cmd_tmp) - index_start - 1);
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
				ft_memmove(&(shell->cmd_tmp[start_quote_index]), &(shell->cmd_tmp[start_quote_index + 1]), ft_strlen(shell->cmd_tmp) - start_quote_index);
				i--;
				ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i);
				i--;
				break ;
			}
		}
	}
	else if (shell->cmd_tmp[i - 1] != ' ')
	{
		ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i); 
		if (!shell->cmd_tmp[i + 1])
			ft_free("Error : unclosed quote\n", shell, 1);
		while (shell->cmd_tmp[++i])
		{
			if (shell->cmd_tmp[i] == quote_type)
			{
				ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i);
				break ;
			}
			else if (!shell->cmd_tmp[i + 1])
				ft_free("Error : unclosed quote\n", shell, 1);
		}
	}
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
