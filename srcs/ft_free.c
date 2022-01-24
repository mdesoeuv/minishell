/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 01:24:22 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/24 14:02:57 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_free(char *message, t_shell *shell, int return_val, int is_exit)
{
	int	i;

	i = -1;
	shell->return_val = return_val;
	ft_putstr(message);
	if (g_sig.readline)
	{
		free(g_sig.readline);
		g_sig.readline = NULL;
	}
	ft_free_cmd(shell);
	if (shell->return_val == 255 || is_exit)
	{
		clear_history();
		free_split(shell->envp);
		exit(shell->return_val);
	}
	return (-1);
}

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

void	ft_free_cmd(t_shell *shell)
{
	t_list_pipes	*cursor;

	if (shell->cmd_tmp)
		free(shell->cmd_tmp);
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
