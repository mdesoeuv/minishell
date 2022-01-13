/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 12:12:21 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/13 14:53:57 by mdesoeuv         ###   ########lyon.fr   */
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

	dprintf(1, "cmd test exe\n");
	ft_print_shell_struct(*shell);
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
	dprintf(1, "cmd executed \n");
	free_split(possible_paths); // code non executé apres le execve
	if (shell->cmd_nbr > 1)
		free(pipe_lst->cmd_path);
}

int	wait_all_pid(t_shell *shell)
{
	int				i;
	t_list_pipes	*pipe_tmp;

	i = 0;
	dprintf(1, "waiting all pid\n");
	pipe_tmp = shell->list_start;
	while (shell->list_start)
	{
		dprintf(1, "waited for %d\n", shell->list_start->pid);
		waitpid(shell->list_start->pid, NULL, 0);
		shell->list_start = shell->list_start->next;
		i++;
	}
	shell->list_start = pipe_tmp;
	dprintf(1, "all pid finished !\n");
	return (0);
}

int	malloc_pipe_fd(t_shell *shell)
{
	int	i;

	i = 0;
	shell->pipe_fd = malloc(sizeof(int *) * (shell->cmd_nbr - 1));
	if (!(shell->pipe_fd))
		return (-1);
	while (i < shell->cmd_nbr - 1)
	{
		shell->pipe_fd[i] = malloc(sizeof(int) * 2);
		if (!(shell->pipe_fd[i]))
			return (-1);
		i++;
	}
	return (0);
}

void	free_fd_tab(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->cmd_nbr - 1)
	{
		free(shell->pipe_fd[i]);
		i++;
	}
	free(shell->pipe_fd);
}
