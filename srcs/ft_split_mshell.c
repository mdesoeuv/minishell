/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_mshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:21:25 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/28 10:46:45 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_split_quotes_variable_replace( t_shell *shell,
	int i, int start_index)
{
	if (shell->cmd_tmp[i] == '$')
	{
		ft_variable_replace(i, shell);
	}
	if (shell->cmd_tmp[i] == '\'' || shell->cmd_tmp[i] == '\"')
	{
		i = ft_parse_quotes(i, start_index,
				shell->cmd_tmp[i], shell);
	}
	else
		i++;
	return (i);
}

static char	**ft_split_quotes_2(char c, t_shell *shell, t_split *split)
{
	while (shell->cmd_tmp[(*split).i])
	{
		if (shell->cmd_tmp[(*split).i] == c)
		{
			if ((*split).i - (*split).start_index > 0)
			{
				(*split).tab[(*split).j] = ft_substr(shell->cmd_tmp,
						(*split).start_index,
						(*split).i - (*split).start_index);
				if ((*split).tab[((*split).j)++] == NULL)
					return (free_return_null((*split).tab, --((*split).j)));
			}
			(*split).start_index = ++((*split).i);
		}
		else
		{
			(*split).i = ft_split_quotes_variable_replace
				(shell, (*split).i, (*split).start_index);
			if ((*split).i == -1)
				ft_free("minishell : syntax error unclosed quote\n",
					shell, 1, 1);
		}
	}
	return ((*split).tab);
}

char	**ft_split_quotes(char c, t_shell *shell)
{
	t_split	split;

	split.i = 0;
	split.j = 0;
	if (malloc_return(&(split.tab), shell->cmd_tmp, c) == NULL)
		return (NULL);
	split.start_index = split.i;
	ft_split_quotes_2(c, shell, &split);
	if (split.i - split.start_index > 0)
	{
		split.tab[split.j] = ft_substr
			(shell->cmd_tmp, split.start_index, split.i - split.start_index);
		if (split.tab[(split.j)++] == NULL)
			return (free_return_null(split.tab, --(split.j)));
	}
	split.tab[split.j] = 0;
	return (split.tab);
}
