/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_mshell.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/27 12:21:25 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/11 12:50:42 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_split_quotes_variable_replace( t_shell *shell,
	int i, int start_index)
{
	if (shell->cmd_tmp[i] == '$')
	{
		ft_variable_replace(i, shell);
		i++;
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
					shell, 258, 1);
		}
	}
	return ((*split).tab);
}

static int	ft_wordcount_mshell(char c, t_shell *shell, t_split *split)
{
	int	count;

	count = 1;
	while (shell->cmd_tmp[(*split).i])
	{
		if (shell->cmd_tmp[(*split).i] == c)
		{
			if ((*split).i - (*split).start_index > 0)
				count++;
			(*split).start_index = ++((*split).i);
			((*split).i)--;
		}
		else if (shell->cmd_tmp[(*split).i] == '\'')
		{
			((*split).i)++;
			while (shell->cmd_tmp[(*split).i]
				&& shell->cmd_tmp[(*split).i] != '\'')
				((*split).i)++;
			((*split).i)--;
		}
		else if (shell->cmd_tmp[(*split).i] == '$')
			ft_variable_replace((*split).i, shell);
		((*split).i)++;
	}
	return (count);
}

char	**malloc_return_mshell(char ***tab,
	char c, t_shell *shell, t_split *split)
{
	if (!shell->cmd_tmp)
		return (NULL);
	*tab = (char **)malloc(sizeof(char *)
			* (ft_wordcount_mshell(c, shell, split) + 1));
	if (!tab)
		return (NULL);
	else
		return (*tab);
}

char	**ft_split_quotes(char c, t_shell *shell)
{
	t_split	split;

	split.i = 0;
	split.j = 0;
	split.start_index = 0;
	if (malloc_return_mshell(&(split.tab), c, shell, &split) == NULL)
		return (NULL);
	split.i = 0;
	split.start_index = 0;
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
