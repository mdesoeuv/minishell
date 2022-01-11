/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 01:24:22 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/11 16:56:25 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(char *message, t_shell	*shell, int is_error)
{
	int	i;

	i = -1;
	ft_putstr(message);
	if (shell->cmd_tmp)
		free(shell->cmd_tmp);
	if (shell->list_start->command && shell->list_start->command[0])
		while (shell->list_start->command[++i])
			free(shell->list_start->command[i]);
	if (shell->list_start->chevron_nbr_in != 0)
		free(shell->list_start->file_in);
	if (shell->list_start->chevron_nbr_out != 0)
		free(shell->list_start->file_out);
	while (shell->list_start->next)
	{
		i = -1;
		if (shell->cmd_tmp)
			free(shell->cmd_tmp);
		if (shell->list_start->command && shell->list_start->command[0])
			while (shell->list_start->command[++i])
				free(shell->list_start->command[i]);
		if (shell->list_start->chevron_nbr_in != 0)
			free(shell->list_start->file_in);
		if (shell->list_start->chevron_nbr_out != 0)
			free(shell->list_start->file_out);
	}
	if (is_error != -1)
		exit(is_error);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split[i]);
	free(split);
}
