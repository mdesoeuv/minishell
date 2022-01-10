/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 10:30:08 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/10 17:03:13 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../srcs/minishell.h"

static char	*ft_split_strdup(char const *s, int i, int len)
{
	char	*copy;
	int		max;
	int		j;

	copy = (char *)malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	max = i + len;
	j = 0;
	while (s[i] && i < max)
	{
		copy[j] = s[i];
		i++;
		j++;
	}
	copy[j] = '\0';
	return (copy);
}

static int	ft_wordcount(char const *s, char c)
{
	int	count;
	int	i;
	int	new_word;

	if (s == NULL)
		return (0);
	i = 0;
	count = 0;
	new_word = 1;
	while (s[i])
	{
		if (s[i] != c && new_word == 1)
		{
			count++;
			new_word = 0;
		}
		if (s[i] == c)
			new_word = 1;
		i++;
	}
	return (count);
}

static char	**malloc_return(char ***tab, char const *s, char c)
{
	*tab = (char **)malloc(sizeof(char *) * (ft_wordcount(s, c) + 1));
	if (!tab)
		return (NULL);
	else
		return (*tab);
}

static char	**free_return_null(char **tab, int j)
{
	while (j-- > 0)
		free(tab[j]);
	free(tab);
	return (NULL);
}

/* s[i] && s[i] != c && i >= size++ used to increment size */
/* and gain one line for norminette check */

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	char	**tab;
	int		size;

	if (malloc_return(&tab, s, c) == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			size = 0;
			while (s[i] && s[i] != c && i >= size++)
				i++;
			tab[j] = ft_split_strdup(s, i - size, size);
			if (tab[j++] == NULL)
				return (free_return_null(tab, --j));
		}
		else
			i++;
	}
	tab[j] = 0;
	return (tab);
}

char	**ft_split_quotes(char c, t_shell *shell)
{
	int		i;
	int		j;
	char	**tab;
	int		start_index;

	if (malloc_return(&tab, shell->cmd_tmp, c) == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (shell->cmd_tmp[i])
	{
		if (shell->cmd_tmp[i] != c)
		{
			start_index = i;
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != c)
			{
				if (shell->cmd_tmp[i] == '$')
					ft_variable_replace(i, shell);
				if (shell->cmd_tmp[i] == '\'' || shell->cmd_tmp[i] == '\"')
					i = ft_parse_quotes(i, start_index, shell->cmd_tmp[i], shell);
				i++;
			}
			if (i - start_index > 1)
			{
				tab[j] = ft_split_strdup(shell->cmd_tmp, start_index, i - start_index);
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
