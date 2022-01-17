/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 01:24:22 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/17 10:11:01 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free(char *message, t_shell	*shell, int is_error)
{
	int	i;

	i = -1;
	ft_putstr(message);
	shell->return_val = is_error;
	if (shell->cmd_nbr > 1)
		free_fd_tab(shell);
	if (shell->cmd_tmp)
		free(shell->cmd_tmp);
	if (shell->list_start->command && shell->list_start->command[0])
		free_split(shell->list_start->command);
		// while (shell->list_start->command[++i])
		// 	free(shell->list_start->command[i]);
	if (shell->list_start->chevron_nbr_in != 0)
		free(shell->list_start->file_in);
	if (shell->list_start->chevron_nbr_out != 0)
		free(shell->list_start->file_out);
	while (shell->list_start->next) // je ne comprends pas cette partie
	{
		i = -1;
		if (shell->cmd_tmp)
			free(shell->cmd_tmp);
		if (shell->list_start->command && shell->list_start->command[0])
			free_split(shell->list_start->command);
			// while (shell->list_start->command[++i])
			// 	free(shell->list_start->command[i]);
		if (shell->list_start->chevron_nbr_in != 0)
			free(shell->list_start->file_in);
		if (shell->list_start->chevron_nbr_out != 0)
			free(shell->list_start->file_out);
		free(shell->list_start->cmd_path);
	}
	if (is_error != -1)
	{
		free_split(shell->envp);
		exit(is_error);
	}
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
	t_list_pipes	*start;

	start = shell->list_start;
	if (shell->cmd_nbr > 1)
		free_fd_tab(shell);
	if (shell->cmd_tmp)
		free(shell->cmd_tmp);
	while (shell->list_start)
	{
		free_split(shell->list_start->command);
		free(shell->list_start->cmd_path);
		free(shell->list_start->file_in);
		free(shell->list_start->file_out);
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = start;
}
