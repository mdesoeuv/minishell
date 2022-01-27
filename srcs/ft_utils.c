/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:10:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/27 12:19:41 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "minishell.h"

static int	ft_split_quotes_variable_replace( t_shell *shell,
	int i, int start_index)
{
	while (i >= 0 && shell->cmd_tmp[i])
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
	}
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
			((*split).i)++;
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
	if (ft_split_quotes_variable_replace
		(shell, split.i, split.start_index) == -1)
		ft_free("Error : unclosed quote\n", shell, 1, 1);
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

int	ft_strisnum(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[0] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

t_list_pipes	*ft_lstnew_pipes(void)
{
	t_list_pipes	*new_elem;

	new_elem = (t_list_pipes *)malloc(sizeof(t_list_pipes));
	if (!new_elem)
		return (NULL);
	new_elem->file_in = NULL;
	new_elem->file_out = NULL;
	new_elem->command = NULL;
	new_elem->cmd_path = NULL;
	new_elem->to_execute = 1;
	new_elem->is_builtin = 1;
	new_elem->chevron_nbr_in = 0;
	new_elem->chevron_nbr_out = 0;
	new_elem->fd_file_in = 0;
	new_elem->fd_file_out = 1;
	new_elem->next = NULL;
	return (new_elem);
}

void	ft_lstadd_back_pipes(t_list_pipes **alst, t_list_pipes *new_elem)
{
	t_list_pipes	*start;

	if (*alst == NULL)
		*alst = new_elem;
	else
	{	
		start = *alst;
		while ((*alst)->next != NULL)
			(*alst) = (*alst)->next;
		(*alst)->next = new_elem;
		*alst = start;
	}
}
