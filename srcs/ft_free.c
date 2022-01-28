/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 01:24:22 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/28 09:26:43 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static void	ft_free_cmd(t_shell *shell)
{
	t_list_pipes	*cursor;

	if (shell->cmd_tmp)
	{
		free(shell->cmd_tmp);
		shell->cmd_tmp = NULL;
	}
	while (shell->list_start)
	{
		cursor = shell->list_start;
		if (shell->list_start->command != NULL)
			free_split(shell->list_start->command);
		if (shell->list_start->cmd_path)
			free(shell->list_start->cmd_path);
		free(shell->list_start->file_in);
		free(shell->list_start->file_out);
		shell->list_start = shell->list_start->next;
		free(cursor);
	}
}

int	ft_free(char *message, t_shell *shell, int g_return_val_param, int is_exit)
{
	int	i;

	i = -1;
	g_return_val = g_return_val_param;
	ft_putstr_fd(message, 2);
	if (shell->readline)
	{
		free(shell->readline);
		shell->readline = NULL;
	}
	ft_free_cmd(shell);
	if (g_return_val == 255 || is_exit)
	{
		clear_history();
		free_split(shell->envp);
		exit(g_return_val);
	}
	return (-1);
}
