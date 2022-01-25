/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:50:13 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/25 11:39:06 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concatenate_path(t_shell *shell, t_list_pipes *pipe_lst, char *path)
{
	pipe_lst->cmd_path = ft_strjoin("", path);
	if (!(pipe_lst->cmd_path))
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	pipe_lst->cmd_path = ft_strjoin_free_s1(pipe_lst->cmd_path, "/");
	if (!(pipe_lst->cmd_path))
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	pipe_lst->cmd_path = ft_strjoin_free_s1(pipe_lst->cmd_path, \
		pipe_lst->command[0]);
	if (!(pipe_lst->cmd_path))
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
}

void	error_cmd_not_found(t_list_pipes *pipe_lst, char **cmd)
{
	int	i;

	pipe_lst->to_execute = 0;
	i = 0;
	ft_putstr_fd("minishell: command not found:", 2);
	while (cmd[i])
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(cmd[i], 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
	if (!(pipe_lst->command[0][0] == '.' || pipe_lst->command[0][0] == '/'))
	{
		free(pipe_lst->cmd_path);
		pipe_lst->cmd_path = NULL;
	}
	else
		pipe_lst->cmd_path = NULL;
	g_sig.exit_status = 127;
}

void	error_cmd_not_executable(t_list_pipes *pipe_lst, char **cmd)
{
	int	i;

	pipe_lst->to_execute = 0;
	i = 0;
	ft_putstr_fd("minishell: permission denied: ", 2);
	while (cmd[i])
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(cmd[i], 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
	if (!(pipe_lst->command[0][0] == '.' || pipe_lst->command[0][0] == '/'))
	{
		free(pipe_lst->cmd_path);
		pipe_lst->cmd_path = NULL;
	}
	else
		pipe_lst->cmd_path = NULL;
	g_sig.exit_status = 126;
}

void	eval_child_status(int child_status)
{
	if (WIFEXITED(child_status) != 0)
		g_sig.exit_status = WEXITSTATUS(child_status);
	else
	{
		if (WIFSIGNALED(child_status) != 0)
			g_sig.exit_status = WTERMSIG(child_status) + 128;
	}
}

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

void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	int		i;
	char	**possible_paths;

	if (pipe_lst->to_execute == 0)
		return ;
	possible_paths = ft_split(getenv("PATH"), ':');
	if (!possible_paths)
		return ;
	if (pipe_lst->command[0][0] != '.' || pipe_lst->command[0][0] != '/')
		pipe_lst->cmd_path = pipe_lst->command[0];
	i = 0;
	while (possible_paths[i] && !(pipe_lst->command[0][0] == '.' \
		|| pipe_lst->command[0][0] == '/'))
	{
		concatenate_path(shell, pipe_lst, possible_paths[i]);
		if (access(pipe_lst->cmd_path, F_OK) == -1)
		{
			free(pipe_lst->cmd_path);
			pipe_lst->cmd_path = NULL;
			i++;
		}
		else
			break ;
	}
	if (!possible_paths[i] || access(pipe_lst->cmd_path, F_OK) == -1)
		error_cmd_not_found(pipe_lst, pipe_lst->command);
	else if (access(pipe_lst->cmd_path, X_OK) == -1)
		error_cmd_not_executable(pipe_lst, pipe_lst->command);
	free_split(possible_paths);
	execve(pipe_lst->cmd_path, pipe_lst->command, shell->envp);
}
