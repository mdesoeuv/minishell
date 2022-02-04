/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 11:45:33 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/04 15:21:54 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_it_builtin(t_list_pipes *pipe_lst)
{
	if (ft_strcmp(pipe_lst->command[0], "pwd") == 0)
		return (1);
	else if (ft_strcmp(pipe_lst->command[0], "cd") == 0)
		return (1);
	else if (ft_strcmp(pipe_lst->command[0], "echo") == 0)
		return (1);
	else if (ft_strcmp(pipe_lst->command[0], "export") == 0)
		return (1);
	else if (ft_strcmp(pipe_lst->command[0], "unset") == 0)
		return (1);
	else if (ft_strcmp(pipe_lst->command[0], "env") == 0)
		return (1);
	else if (ft_strcmp(pipe_lst->command[0], "exit") == 0)
		return (1);
	else
	{
		pipe_lst->is_builtin = 0;
		return (0);
	}
}

static int	exec_builtin_fork(t_shell *shell, t_list_pipes *pipe_lst)
{
	pipe_lst->pid = fork();
	if (pipe_lst->pid < 0)
		ft_free("minishell: fork error\n", shell, 1, 1);
	else if (pipe_lst->pid == 0)
	{
		if (ft_strcmp(pipe_lst->command[0], "pwd") == 0)
			g_return_val = print_working_directory();
		else if (ft_strcmp(pipe_lst->command[0], "cd") == 0)
			g_return_val = change_directory(shell, pipe_lst->command[1]);
		else if (ft_strcmp(pipe_lst->command[0], "echo") == 0)
			g_return_val = ft_echo(shell);
		else if (ft_strcmp(pipe_lst->command[0], "export") == 0)
			g_return_val = ft_export_multi(shell, pipe_lst->command);
		else if (ft_strcmp(pipe_lst->command[0], "unset") == 0)
			g_return_val = ft_unset_multi(shell, pipe_lst->command);
		else if (ft_strcmp(pipe_lst->command[0], "env") == 0)
			g_return_val = ft_env(shell, pipe_lst, pipe_lst->command[1]);
		else if (ft_strcmp(pipe_lst->command[0], "exit") == 0)
		{
			shell->is_exit = ft_exit(shell, pipe_lst);
			g_return_val = shell->is_exit;
		}
		exit(g_return_val);
	}
	return (0);
}

static int	exec_builtin(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (ft_strcmp(pipe_lst->command[0], "pwd") == 0)
		g_return_val = print_working_directory();
	else if (ft_strcmp(pipe_lst->command[0], "cd") == 0)
		g_return_val = change_directory(shell, pipe_lst->command[1]);
	else if (ft_strcmp(pipe_lst->command[0], "echo") == 0)
		g_return_val = ft_echo(shell);
	else if (ft_strcmp(pipe_lst->command[0], "export") == 0)
		g_return_val = ft_export_multi(shell, pipe_lst->command);
	else if (ft_strcmp(pipe_lst->command[0], "unset") == 0)
		g_return_val = ft_unset_multi(shell, pipe_lst->command);
	else if (ft_strcmp(pipe_lst->command[0], "env") == 0)
		g_return_val = ft_env(shell, pipe_lst, pipe_lst->command[1]);
	else if (ft_strcmp(pipe_lst->command[0], "exit") == 0)
	{
		shell->is_exit = ft_exit(shell, pipe_lst);
		g_return_val = shell->is_exit;
	}
	return (g_return_val);
}

int	execute_if_built_in(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (!pipe_lst->command[0])
		return (-999);
	if (pipe_lst->to_ex == 0)
		return (-100);
	if (is_it_builtin(pipe_lst) == 1)
	{
		if (shell->cmd_nbr > 1)
			return (exec_builtin_fork(shell, pipe_lst));
		else
			return (exec_builtin(shell, pipe_lst));
	}
	return (-100);
}
