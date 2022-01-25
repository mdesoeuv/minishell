/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:10:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/25 15:15:44 by vchevill         ###   ########lyon.fr   */
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
			i++;
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

char	**ft_split_quotes(char c, t_shell *shell, int i, int j)
{
	char	**tab;
	int		start_index;

	if (malloc_return(&tab, shell->cmd_tmp, c) == NULL)
		return (NULL);
	start_index = i;
	if (ft_split_quotes_variable_replace(shell, i, start_index) == -1)
		ft_free("Error : unclosed quote\n", shell, 1, 1);
	while (shell->cmd_tmp[i])
	{
		if (shell->cmd_tmp[i] == c)
		{
			if (i - start_index > 0)
			{
				tab[j] = ft_substr(shell->cmd_tmp,
						start_index, i - start_index);
				if (tab[j++] == NULL)
					return (free_return_null(tab, --j));
			}
			start_index = ++i;
		}
		else
			i++;
	}
	if (i - start_index > 0)
	{
		tab[j] = ft_substr(shell->cmd_tmp, start_index, i - start_index);
		if (tab[j++] == NULL)
			return (free_return_null(tab, --j));
	}
	tab[j] = 0;
	return (tab);
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
