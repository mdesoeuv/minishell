/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 11:45:33 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/25 11:46:18 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_it_builtin(t_list_pipes *pipe_lst)
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
		return (0);
}

int	exec_builtin_fork(t_shell *shell, t_list_pipes *pipe_lst)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_free("minishell: fork error\n", shell, 1, 1);
	else if (pid == 0)
	{
		if (ft_strcmp(pipe_lst->command[0], "pwd") == 0)
			print_working_directory();
		else if (ft_strcmp(pipe_lst->command[0], "cd") == 0)
			change_directory(shell, pipe_lst->command[1]);
		else if (ft_strcmp(pipe_lst->command[0], "echo") == 0)
			ft_echo(shell);
		else if (ft_strcmp(pipe_lst->command[0], "export") == 0)
			ft_export(shell, pipe_lst->command[1]);
		else if (ft_strcmp(pipe_lst->command[0], "unset") == 0)
			ft_unset(shell, pipe_lst->command[1]);
		else if (ft_strcmp(pipe_lst->command[0], "env") == 0)
			ft_env(shell, pipe_lst->command[1]);
		else if (ft_strcmp(pipe_lst->command[0], "exit") == 0)
			shell->is_exit = ft_exit(shell);
		exit(0);
	}
	return (0);
}

int	exec_builtin(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (ft_strcmp(pipe_lst->command[0], "pwd") == 0)
		return (print_working_directory());
	else if (ft_strcmp(pipe_lst->command[0], "cd") == 0)
		return (change_directory(shell, pipe_lst->command[1]));
	else if (ft_strcmp(pipe_lst->command[0], "echo") == 0)
		return (ft_echo(shell));
	else if (ft_strcmp(pipe_lst->command[0], "export") == 0)
		return (ft_export(shell, pipe_lst->command[1]));
	else if (ft_strcmp(pipe_lst->command[0], "unset") == 0)
		return (ft_unset(shell, pipe_lst->command[1]));
	else if (ft_strcmp(pipe_lst->command[0], "env") == 0)
		return (ft_env(shell, pipe_lst->command[1]));
	else if (ft_strcmp(pipe_lst->command[0], "exit") == 0)
	{
		shell->is_exit = ft_exit(shell);
		return (0);
	}
	return (0);
}

int	execute_if_built_in(t_shell *shell, t_list_pipes *pipe_lst)
{
	if (!pipe_lst->command[0])
		return (-999);
	if (pipe_lst->to_execute == 0)
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
