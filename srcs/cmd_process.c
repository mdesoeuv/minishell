/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:50:13 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/13 15:36:34 by mdesoeuv         ###   ########lyon.fr   */
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
	// print_split(possible_paths);
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
	free_split(possible_paths); // code non executé apres le execve
	if (shell->cmd_nbr > 1)
		free(pipe_lst->cmd_path);
}

int	cmd_process(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_lst_tmp;

	pipe_lst_tmp = shell->list_start;
	if (shell->cmd_nbr > 1)
		if (malloc_pipe_fd(shell) == -1)
			return (-1);
	i = 0;
	manage_all_file_fd(shell);
	while (i < shell->cmd_nbr)
	{
		// dprintf(1, "\n== loop %d ==\n\n", i);
		ft_print_shell_struct(*shell);
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
			// dprintf(1, "==child fork executing cmd %d => %s\n", i, shell->list_start->command[0]);
			manage_dup_fd(shell, shell->list_start, i);
			cmd_test_execute(shell, shell->list_start);
			dprintf(1, "child fork cmd %d not executed !==\n", i);
			exit(0); // to replace
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
	// close_file_pipes(shell);
	if (shell->cmd_nbr > 1)
		free_fd_tab(shell);
	return (0);
}
