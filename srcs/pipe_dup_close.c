/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_dup_close.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:45:22 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/24 10:28:57 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_unused_pipes(t_shell *shell, t_list_pipes *pipe_lst, int i)
{
	if (i > 0)
		close(shell->pipe_fd[i - 1][1]);
	close(shell->pipe_fd[i][0]);
	if (pipe_lst->file_in != NULL)
		close(shell->pipe_fd[i - 1][0]);
	if (pipe_lst->file_out != NULL)
		close(shell->pipe_fd[i][1]);
}

int	manage_all_file_fd(t_shell *shell)
{
	t_list_pipes	*lst_start;

	lst_start = shell->list_start;
	while (shell->list_start)
	{
		if (shell->list_start->file_in != NULL)
		{
			if (shell->list_start->chevron_nbr_in == 1)
				shell->list_start->fd_file_in = \
				open(shell->list_start->file_in, O_RDONLY, S_IRWXU);
			else if (shell->list_start->chevron_nbr_in > 1)
			{
				shell->list_start->fd_file_in = here_doc_v2(shell, shell->list_start);
			}
		}
		if (shell->list_start->file_out != NULL)
		{
			if (shell->list_start->chevron_nbr_out == 1)
				shell->list_start->fd_file_out = open(shell->\
				list_start->file_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				shell->list_start->fd_file_out = open(shell->\
				list_start->file_out, O_WRONLY | O_APPEND | O_CREAT, 0644);
		}
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = lst_start;
	if (shell->list_start->fd_file_in < 0 || shell->list_start->fd_file_out < 0)
		perror("minishell");
	return (0);
}

/* S_IRWXU ? */

int	manage_file_fd(t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in != NULL)
	{
		if (pipe_lst->chevron_nbr_in == 1)
		{
			pipe_lst->fd_file_in = open(pipe_lst->file_in, O_RDONLY, S_IRWXU);
		}
		else if (pipe_lst->chevron_nbr_in > 1)
		{
			ft_putstr("<< not yet managed\n");
			pipe_lst->fd_file_in = 0;
		}
	}
	if (pipe_lst->fd_file_in < 0)
	{
		dprintf(2, "FILE OPENING ERROR : %s\n", pipe_lst->file_in);
		close(pipe_lst->fd_file_in);
		exit(0);
	}
	if (pipe_lst->file_out != NULL)
	{
		if (pipe_lst->chevron_nbr_out == 1)
			pipe_lst->fd_file_out = open(pipe_lst->file_out, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644); // S_IRWXU
		else
			pipe_lst->fd_file_out = open(pipe_lst->file_out, \
				O_WRONLY | O_CREAT | O_APPEND, 0644); // S_IRWXU
	}
	// if (pipe_lst->fd_file_in == -1)
	// 	perror("minishell");
	return (0);
}

int	manage_dup_fd(t_shell *shell, t_list_pipes *pipe_lst, int i)
{
	if (i > 0 && shell->cmd_nbr > 1)
		close(shell->pipe_fd[i - 1][1]);
	if (shell->cmd_nbr > 1 && i < shell->cmd_nbr - 1)
		close(shell->pipe_fd[i][0]);
	if (pipe_lst->file_in != NULL)
	{
		dup2(pipe_lst->fd_file_in, 0);
		close(pipe_lst->fd_file_in);
		if (i > 0 && shell->cmd_nbr > 1)
			close(shell->pipe_fd[i - 1][0]);
	}
	else if (i > 0 && shell->cmd_nbr > 1)
	{
		dup2(shell->pipe_fd[i - 1][0], 0);
		// close(shell->pipe_fd[i - 1][0]);
	}
	if (pipe_lst->file_out != NULL)
	{
		dup2(pipe_lst->fd_file_out, 1);
		close(pipe_lst->fd_file_out);
		if (shell->cmd_nbr > 1 && i < shell->cmd_nbr - 1)
			close(shell->pipe_fd[i][1]);
	}
	else if (i < shell->cmd_nbr - 1 && shell->cmd_nbr > 1)
	{
		dup2(shell->pipe_fd[i][1], 1);
		// close(shell->pipe_fd[i][1]);
	}
	return (0);
}

int	close_all_dup_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->cmd_nbr - 1)
	{
		close(shell->pipe_fd[i][0]);
		close(shell->pipe_fd[i][1]);
		i++;
	}
	return (0);
}

int	close_dup_pipes(t_shell *shell)
{
	int				i;
	t_list_pipes	*start_lst;

	start_lst = shell->list_start;
	i = 0;
	while (shell->list_start != NULL)
	{
		if (shell->list_start->file_in == NULL && (shell->cmd_nbr > 1 && i > 0))
			close(shell->pipe_fd[i - 1][0]);
		if (shell->list_start->file_out == NULL && \
			(shell->cmd_nbr > 1 && i < shell->cmd_nbr - 1))
			close(shell->pipe_fd[i][1]);
		shell->list_start = shell->list_start->next;
		i++;
	}
	shell->list_start = start_lst;
	return (0);
}

int	close_file_pipes(t_shell *shell)
{
	int				i;
	t_list_pipes	*start_lst;

	//ft_print_shell_struct(*shell);
	start_lst = shell->list_start;
	i = 0;
	while (shell->list_start != NULL)
	{
		if (shell->list_start->file_in != NULL)
			close(shell->list_start->fd_file_in);
		if (shell->list_start->file_out != NULL)
			close(shell->list_start->fd_file_out);
		shell->list_start = shell->list_start->next;
		i++;
	}
	shell->list_start = start_lst;
	return (0);
}
