/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:10:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/11 17:12:38 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../srcs/minishell.h"

static int	ft_split_quotes_variable_replace(char c, t_shell *shell,
	int i, int start_index)
{
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != c)
	{
		if (shell->cmd_tmp[i] == '$')
			ft_variable_replace(i, shell);
		if (shell->cmd_tmp[i] == '\'' || shell->cmd_tmp[i] == '\"')
			i = ft_parse_quotes(i, start_index,
					shell->cmd_tmp[i], shell);
		i++;
	}
	return (i);
}

char	**ft_split_quotes(char c, t_shell *shell, int i, int j)
{
	char	**tab;
	int		start_index;

	if (malloc_return(&tab, shell->cmd_tmp, c) == NULL)
		return (NULL);
	while (shell->cmd_tmp[i])
	{
		if (shell->cmd_tmp[i] != c)
		{
			start_index = i;
			i = ft_split_quotes_variable_replace(c, shell, i, start_index);
			if (i - start_index > 1)
			{
				tab[j] = ft_split_strdup(shell->cmd_tmp,
						start_index, i - start_index);
				if (tab[j++] == NULL)
					return (free_return_null(tab, --j));
			}
		}
		else
			i++;
	}
	tab[j] = 0;
	return (tab);
}
