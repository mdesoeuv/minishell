/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 11:32:38 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/25 12:12:16 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	pipe_lst->pid = fork();
	if (pipe_lst->pid < 0)
	{
		ft_free("minishell: fork error\n", shell, 1, 1);
	}
	else if (pipe_lst->pid == 0)
	{
		if (pipe_lst->to_execute == 1)
			cmd_test_execute(shell, pipe_lst);
		ft_free("", shell, 1, 1);
	}
}

void	end_cmd_process(t_shell *shell)
{
	wait_all_pid(shell);
	close_file_pipes(shell);
	restore_fd(shell);
	g_sig.pid = 0;
}

void	new_cmd_process(t_shell *shell)
{
	t_list_pipes	*start;
	int				i;
	int				pipe_fd[2];
	int				fd_prev_pipe;

	i = 0;
	start = shell->list_start;
	init_fd(shell);
	open_in_out_all(shell);
	fd_prev_pipe = 0;
	while (shell->list_start)
	{
		shell->list_start->cmd_index = i++;
		g_sig.pid = 1;
		if (pipe(pipe_fd) == -1)
			ft_free("minishell: pipe error\n", shell, 1, 1);
		fd_prev_pipe = fd_redirect(shell, shell->list_start, \
			fd_prev_pipe, pipe_fd);
		redirect_file_in_out(shell->list_start);
		if (shell->list_start->to_execute == 1)
		{
			if (execute_if_built_in(shell, shell->list_start) == -100)
				execute(shell, shell->list_start);
		}
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->list_start = shell->list_start->next;
	}
	shell->list_start = start;
	end_cmd_process(shell);
}
