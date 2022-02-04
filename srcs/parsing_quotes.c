/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 11:47:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/04 14:32:30 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_parse_quotes(int i, int index_start,
	char quote_type, t_shell *shell)
{
	int	start_quote_index;

	index_start = i;
	start_quote_index = i;
	while (shell->cmd_tmp[++i])
	{
		if (quote_type == '\"' && shell->cmd_tmp[i] == '$')
			ft_variable_replace(i, shell);
	}
	i = index_start;
	while (shell->cmd_tmp[++i])
	{
		if (shell->cmd_tmp[i] == quote_type)
		{
			ft_memmove(&(shell->cmd_tmp[start_quote_index]),
				&(shell->cmd_tmp[start_quote_index + 1]),
				ft_strlen(shell->cmd_tmp) - start_quote_index);
			i--;
			ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
				ft_strlen(shell->cmd_tmp) - i);
			break ;
		}
	}
	return (i);
}
