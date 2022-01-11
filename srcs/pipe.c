/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/11 11:40:43 by mdesoeuv         ###   ########lyon.fr   */
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
	dprintf(1, "concatened path = %s\n", pipe_lst->cmd_path);
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

void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	int		i;
	char	**possible_paths;

	possible_paths = ft_split(getenv("PATH"), ':');
	if (!possible_paths)
		return ;
	print_split(possible_paths);
	i = 0;
	while (possible_paths[i])
	{
		concatenate_path(pipe_lst, possible_paths[i]);
		dprintf(1, "path tested = %s\n", pipe_lst->cmd_path);
		if (access(pipe_lst->cmd_path, F_OK) == -1)
		{
			free(pipe_lst->cmd_path);
			i++;
		}
		else
			break ;
	}
	if (!possible_paths[i])
		error_cmd_not_found(pipe_lst->command);
	else if (execve(pipe_lst->cmd_path, pipe_lst->command, shell->envp) == -1)
		perror("minishell");
	free_split(possible_paths);
	free(pipe_lst->cmd_path);
}

int	manage_file_fd(t_shell *shell, t_list_pipes *pipe_lst, int i)
{
	if (pipe_lst->file_in != NULL)
	{
		if (i > 0)
			close(shell->pipe_fd[i - 1][0]);
		if (pipe_lst->chevron_nbr_in == 1)
			pipe_lst->fd_file_in = open(pipe_lst->file_in, O_RDONLY);
		else if (pipe_lst->chevron_nbr_in > 1)
		{
			ft_putstr("<< not yet managed\n");
			pipe_lst->fd_file_in = 0;
		}
	}
	if (pipe_lst->fd_file_in == -1)
		perror("minishell");
	if (pipe_lst->file_out != NULL)
	{
		if (i < shell->pipes_nbr - 1)
			close(shell->pipe_fd[i][1]);
		if (pipe_lst->chevron_nbr_out == 1)
			pipe_lst->fd_file_out = open(pipe_lst->file_out, \
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			pipe_lst->fd_file_out = open(pipe_lst->file_out, \
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
	else if (i > 0)
		dup2(shell->pipe_fd[i - 1][0], 0);
	if (pipe_lst->file_out != NULL)
		dup2(pipe_lst->fd_file_out, 1);
	else if (i < shell->pipes_nbr - 1)
		dup2(shell->pipe_fd[i][1], 1);
	return (0);
}

int	close_all_pipes(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->pipes_nbr - 1)
	{
		close(shell->pipe_fd[i][0]);
		close(shell->pipe_fd[i][1]);
		i++;
	}
	while (shell->list_start != NULL)
	{
		if (shell->list_start->file_in != NULL)
			close(shell->list_start->fd_file_in);
		if (shell->list_start->file_out != NULL)
			close(shell->list_start->fd_file_out);
		shell->list_start = shell->list_start->next;
	}
	return (0);
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

int	cmd_process(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_lst;

	pipe_lst = shell->list_start;
	if (malloc_pipe_fd(shell) == -1)
		return (-1);
	i = 0;
	while (i < shell->pipes_nbr) // while (pipe_lst != NULL)
	{
		dprintf(1, "infile = %s\n", pipe_lst->file_in);
		dprintf(1, "outfile = %s\n", pipe_lst->file_out);
		if (i < shell->pipes_nbr - 1)
			pipe(shell->pipe_fd[i]);
		pipe_lst->pid = fork();
		if (pipe_lst->pid < 0)
		{
			perror("minishell");
			exit(EXIT_FAILURE); // to replace with ft_exit
		}
		else if (pipe_lst->pid == 0)
		{
			manage_file_fd(shell, pipe_lst, i);
			close_unused_pipes(shell, i);
			manage_dup_fd(shell, pipe_lst, i);
			cmd_test_execute(shell, pipe_lst);
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
	// ft_lstclear(&(shell->list_start), free);
	return (0);
}
