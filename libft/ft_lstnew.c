/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/14 15:18:12 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/10 17:59:56 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_elem;

	new_elem = (t_list *)malloc(sizeof(t_list));
	if (!new_elem)
		return (NULL);
	if (content == NULL)
		new_elem->content = NULL;
	else
		new_elem->content = content;
	new_elem->next = NULL;
	return (new_elem);
}

t_list_pipes	*ft_lstnew_pipes(void)
{
	t_list_pipes	*new_elem;

	new_elem = (t_list_pipes *)malloc(sizeof(t_list_pipes));
	if (!new_elem)
		return (NULL);
	new_elem->file_in = NULL;
	new_elem->file_out = NULL;
	new_elem->next = NULL;
	return (new_elem);
}
