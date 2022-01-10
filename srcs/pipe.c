/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/10 10:31:47 by mdesoeuv         ###   ########lyon.fr   */
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

int	manage_cmd_fd(t_list_pipes *pipe_lst)
{
	
}

int	cmd_process(t_shell *shell, t_list_pipes *pipe_lst)
{
	int	i;

	i = 0;
	while (i < shell->pipes_nbr + 1) // while (pipe_lst != NULL)
	{
		pipe(shell->pipe_fd[i]);
		shell->pid[i] = fork();
		if (shell->pid[i] < 0)
		{
			perror("minishell");
			exit(EXIT_FAILURE); // to replace with ft_exit
		}
		else if (shell->pid[i] == 0)
		{
			manage_cmd_fd(pipe_lst);
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
