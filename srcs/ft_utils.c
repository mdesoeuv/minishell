/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 17:10:33 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/02 11:48:39 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	ft_putstr_fd_shell(char *s, int fd)
{
	if (!s)
		return (-1);
	write(fd, s, ft_strlen(s));
	g_return_val = 258;
	return (-1);
}
