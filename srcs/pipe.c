/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/10 15:21:01 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concatenate_path(t_list_pipes *pipe_lst, char *path)
{
	pipe_lst->cmd_path = ft_strjoin_free_s2("", path);
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

void	error_cmd_not_found(char **cmd)
{
	int	i;

	i = 0;
	ft_putstr_fd("pipex: command not found:", 2);
	while (cmd[i])
	{
		ft_putstr_fd(" ", 2);
		ft_putstr_fd(cmd[i], 2);
		i++;
	}
	ft_putstr_fd("\n", 2);
}

void	cmd_test_execute(t_list_pipes *pipe_lst, char **envp)
{
	int		i;
	char	**possible_paths;

	possible_paths = ft_split(getenv("PATH"), ':');
	if (!possible_paths)
		return ;
	i = 0;
	while (possible_paths[i])
	{
		concatenate_path(pipe_lst, possible_paths[i]);
		if (access(pipe_lst->cmd_path, F_OK) == -1)
		{
			free(pipe_lst->cmd_path);
			i++;
		}
		else
			break ;
	}
	free_split(possible_paths);
	if (!possible_paths[i])
		error_cmd_not_found(pipe_lst->command[0]);
	else if (execve(pipe_lst->cmd_path, pipe_lst->command, envp) == -1)
		perror("minishell");
}

int	manage_file_fd(t_list_pipes *pipe_lst)
{
	if (pipe_lst->file_in != NULL)
	{
		if (pipe_lst->file_in_overwrite == 0)
			pipe_lst->fd_file_in = open(pipe_lst->file_in, O_RDONLY);
		else
		{
			ft_putstr("<< not yet managed\n");
			pipe_lst->fd_file_in == 0;
		}
	}
	if (pipe_lst->fd_file_in == -1)
		perror("minishell");
	if (pipe_lst->file_out != NULL)
	{
		if (pipe_lst->file_out_overwrite == 0)
			pipe_lst->fd_file_out = open(pipe_lst->file_in, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			pipe_lst->fd_file_out = open(pipe_lst->file_in, \
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	if (pipe_lst->fd_file_in == -1)
		perror("minishell");
	return (0);
}

int	manage_dup_fd(t_shell *shell, t_list_pipes *pipe_lst, int i)
{
	if (pipe_lst->file_in != NULL)
		dup2(pipe_lst->fd_file_in, 0);
	else if (i != 0)
		dup2(pipe_lst->pipe_fd[0], 0);
	if (pipe_lst->file_out != NULL)
		dup2(pipe_lst->fd_file_out, 0);
	else if (i != shell->pipes_nbr)
		dup2(pipe_lst->pipe_fd[1], 1);
	return (0);
}

int	close_all_pipes(t_shell *shell)
{
	while (shell->list_start != NULL)
	{
		close(shell->list_start->pipe_fd[0]);
		close(shell->list_start->pipe_fd[1]);
		if (shell->list_start->file_in != NULL)
			close(shell->list_start->fd_file_in);
		if (shell->list_start->file_out != NULL)
			close(shell->list_start->fd_file_out);
		shell->list_start = shell->list_start->next;
	}
}

int	wait_all_pid(t_shell *shell)
{
	while (shell->list_start)
	{
		waitpid(shell->list_start->pid, NULL, 0);
		shell->list_start = shell->list_start->next;
	}
	return (0);
}

int	cmd_process(t_shell *shell, t_list_pipes *pipe_lst)
{
	int	i;

	i = 0;
	while (i < shell->pipes_nbr) // while (pipe_lst != NULL)
	{
		pipe(pipe_lst->pipe_fd);
		pipe_lst->pid = fork();
		if (pipe_lst->pid < 0)
		{
			perror("minishell");
			exit(EXIT_FAILURE); // to replace with ft_exit
		}
		else if (pipe_lst->pid == 0)
		{
			manage_file_fd(pipe_lst);
			manage_dup_fd(shell, pipe_lst, i);
			test_execute_cmd(pipe_lst->command[i]);
			return (0);
		}
		else
		{
			i++;
			pipe_lst = pipe_lst->next;
		}
	}
	close_all_pipes(shell);
	wait_all_pid(shell);
}
