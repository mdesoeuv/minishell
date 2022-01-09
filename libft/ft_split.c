/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 10:30:08 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/09 04:23:25 by vchevill         ###   ########.fr       */
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

char *	ft_variable_replace(char *command, int i, t_shell *shell)
{
	int		index_start;
	char	*variable_name;
	char	*variable_result;
	char	*tmp;
	char	*new_command;
	
	index_start = i + 1;
	while (command[i] && command[i] != ' ' && command[i] != '\"')
		i++;
	variable_name = ft_substr(command, index_start, i - index_start);// checker les variables d'env dans des guillemets
	if (!variable_name)
		ft_free("Error : malloc error\n", shell, 1);
	variable_result = getenv(variable_name);
	free(variable_name);
	if (variable_result)
	{
		ft_memmove(&command[index_start - 1], &command[i],
			ft_strlen(command) - index_start - 1);
		tmp = ft_strndup(command, index_start - 1);
		if (!tmp)
			ft_free("Error : malloc error\n", shell, 1);
		new_command = ft_strjoin(tmp, variable_result);
		if (!new_command)
			ft_free("Error : malloc error\n", shell, 1);
		new_command = ft_strjoin(new_command, ft_substr(command, index_start - 1, ft_strlen(command) - index_start + 1));
		if (!new_command)
			ft_free("Error : malloc error\n", shell, 1);
		free(tmp);
		return (new_command);
	}
	ft_memmove(&command[index_start], &command[i], ft_strlen(command) - index_start);
	return (command);
}
char	**ft_split_quotes(char *s, char c, t_shell *shell)
{
	int		i;
	int		j;
	char	**tab;
	int		size;
	int		is_quote;

	if (malloc_return(&tab, s, c) == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			is_quote = 0;
			size = 0;
			while (s[i] && s[i] != c && i >= size++)
			{
				if (s[i] == '\'')
				{
					is_quote = 1;
					while (s[++i] && i >= size++)
						if (s[i] == '\'')
							break ;
				}
				else if (s[i] == '\"')
				{
					is_quote = 1;
					while (s[++i] && i >= size++)
						if (s[i] == '$')
							s = ft_variable_replace(s, i, shell);
						else if (s[i] == '\"')
							break ;
				}
				else if (s[i] == '$')
					s = ft_variable_replace(s, i, shell);
				i++;
			}
			if (is_quote == 1 && size - 2 > 1)
			{
				tab[j] = ft_split_strdup(s, i - size + 1, size - 2);
				if (tab[j++] == NULL)
					return (free_return_null(tab, --j));
			}
			else if (size > 1)
			{
				tab[j] = ft_split_strdup(s, i - size, size);
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
