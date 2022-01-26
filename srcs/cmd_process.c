/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:50:13 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/26 14:14:36 by vchevill         ###   ########.fr       */
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
	g_return_val = 127;
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
	g_return_val = 126;
}

void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	int		i;
	char	**possible_paths;

	if (pipe_lst->to_execute == 0)
		return ;
	if (ft_getenv(shell, "PATH") == NULL)
	{
		error_cmd_not_found(pipe_lst, pipe_lst->command);
		return ;
	}
	possible_paths = ft_split(ft_getenv(shell, "PATH"), ':');
	if (!possible_paths)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
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
