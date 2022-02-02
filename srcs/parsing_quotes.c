/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 11:47:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/02 11:33:07 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_parse_quotes_unclosed(int i, char quote_type, t_shell *shell)
{
	ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
		ft_strlen(shell->cmd_tmp) - i);
	if (!shell->cmd_tmp[i + 1])
		return (ft_free("minishell : syntax error unclosed quote\n",
				shell, 258, 0));
	while (shell->cmd_tmp[++i])
	{
		if (shell->cmd_tmp[i] == quote_type)
		{
			ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
				ft_strlen(shell->cmd_tmp) - i);
			break ;
		}
		else if (!shell->cmd_tmp[i + 1])
			return (ft_free("minishell : syntax error unclosed quote\n",
					shell, 258, 0));
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
