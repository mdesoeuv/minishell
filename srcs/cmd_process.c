/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:50:13 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/18 09:36:55 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concatenate_path(t_list_pipes *pipe_lst, char *path)
{
	pipe_lst->cmd_path = ft_strjoin("", path);
	if (!(pipe_lst->cmd_path))
		exit(EXIT_FAILURE); // to replace with built_in function
	pipe_lst->cmd_path = ft_strjoin_free_s1(pipe_lst->cmd_path, "/");
	if (!(pipe_lst->cmd_path))
		exit(EXIT_FAILURE); // to replace with built_in function
	pipe_lst->cmd_path = ft_strjoin_free_s1(pipe_lst->cmd_path, \
		pipe_lst->command[0]);
	if (!(pipe_lst->cmd_path))
		exit(EXIT_FAILURE); // to replace with built_in function
}

void	error_cmd_not_found(char **cmd, char **possible_paths)
{
	int	i;

	i = 0;
	ft_putstr_fd("minishell: command not found:", 2);
	while (cmd[i])
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(cmd[i], 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
	free_split(possible_paths);
}

/*	returns 0 if not built-in, else returns function's index */

int	execute_if_built_in(t_shell *shell, t_list_pipes *pipe_lst)
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
		shell->is_exit = ft_exit(shell); // TO DO exit return value
		shell->return_val = shell->is_exit;
		return (shell->return_val);
	}
	return (-100);
}

void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	int		i;
	char	**possible_paths;

	// shell->return_val = execute_if_built_in(shell, pipe_lst);
	// if (shell->return_val != -100)
	// 	return ;
	possible_paths = ft_split(getenv("PATH"), ':');
	if (!possible_paths)
		return ;
	if (pipe_lst->command[0][0] != '.' || pipe_lst->command[0][0] != '/')
		pipe_lst->cmd_path = pipe_lst->command[0];
	i = 0;
	while (possible_paths[i] && !(pipe_lst->command[0][0] == '.' \
		|| pipe_lst->command[0][0] == '/'))
	{
		concatenate_path(pipe_lst, possible_paths[i]);
		// dprintf(1, "path tested = %s\n", pipe_lst->cmd_path);
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
		return (error_cmd_not_found(pipe_lst->command, possible_paths));
	free_split(possible_paths);
	if (execve(pipe_lst->cmd_path, pipe_lst->command, shell->envp) == -1)
		perror("minishell");
}

int	cmd_process(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_lst_tmp;

	pipe_lst_tmp = shell->list_start;
	malloc_pipe_fd(shell);
	manage_all_file_fd(shell);
	i = 0;
	while (i < shell->cmd_nbr)
	{
		if (shell->cmd_nbr > 1 && i < shell->cmd_nbr - 1)
			pipe(shell->pipe_fd[i]);
		shell->list_start->pid = fork();
		if (shell->list_start->pid < 0)
		{
			perror("minishell");
			ft_exit(shell);
		}
		else if (shell->list_start->pid == 0)
		{
			manage_dup_fd(shell, shell->list_start, i);
			cmd_test_execute(shell, shell->list_start);
			ft_free("", shell, shell->return_val, 1);
		}
		else
		{
			if (shell->cmd_nbr > 1 && i < shell->cmd_nbr && i > 0)
			{
				close(shell->pipe_fd[i - 1][0]);
				close(shell->pipe_fd[i - 1][1]);
			}
			i++;
			shell->list_start = shell->list_start->next;
		}
	}
	shell->list_start = pipe_lst_tmp;
	wait_all_pid(shell);
	close_file_pipes(shell);
	return (0);
}
