/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_process.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/13 14:50:13 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/04 15:22:16 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	concatenate_path(t_shell *shell, t_list_pipes *pipe_lst, char *path)
{
	pipe_lst->cmd_pa = ft_strjoin("", path);
	if (!(pipe_lst->cmd_pa))
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	pipe_lst->cmd_pa = ft_strjoin_free_s1(pipe_lst->cmd_pa, "/");
	if (!(pipe_lst->cmd_pa))
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	pipe_lst->cmd_pa = ft_strjoin_free_s1(pipe_lst->cmd_pa, \
		pipe_lst->command[0]);
	if (!(pipe_lst->cmd_pa))
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
}

static void	error_cmd_not_executable(t_list_pipes *pipe_lst, char **cmd)
{
	pipe_lst->to_ex = 0;
	ft_putstr_fd("minishell:", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": permission denied\n", 2);
	if (!(pipe_lst->command[0][0] == '.' || pipe_lst->command[0][0] == '/'))
	{
		free(pipe_lst->cmd_pa);
		pipe_lst->cmd_pa = NULL;
	}
	else
		pipe_lst->cmd_pa = NULL;
	g_return_val = 126;
}

void	cmd_concatenate_test(t_shell *shell, char **p, \
	t_list_pipes *pipe_lst, int *i)
{
	*i = 0;
	while (p[*i] && !(pipe_lst->command[0][0] == '.' \
		|| pipe_lst->command[0][0] == '/'))
	{
		concatenate_path(shell, pipe_lst, p[*i]);
		if (access(pipe_lst->cmd_pa, F_OK) == -1)
		{
			if (!(pipe_lst->command[0][0] == '.' || \
				pipe_lst->command[0][0] == '/'))
				free(pipe_lst->cmd_pa);
			pipe_lst->cmd_pa = NULL;
			*i += 1;
		}
		else
			break ;
	}
}

void	cmd_test_execute(t_shell *shell, t_list_pipes *pipe_lst)
{
	int		i;
	char	**p;

	if (ft_getenv(shell, "PATH") != NULL)
		p = ft_split(ft_getenv(shell, "PATH"), ':');
	else
		p = ft_split(" ", ':');
	if (!p)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	if (pipe_lst->command[0][0] != '.' || pipe_lst->command[0][0] != '/')
	{
		pipe_lst->cmd_pa = ft_strdup(pipe_lst->command[0]);
		if (!pipe_lst->cmd_pa)
		{
			free_split(p);
			ft_free("minishell: memory allocation error\n", shell, 1, 1);
		}
	}
	cmd_concatenate_test(shell, p, pipe_lst, &i);
	if (pipe_lst->to_ex == 1 && (!p[i] || access(pipe_lst->cmd_pa, F_OK) == -1))
		error_cmd_not_found(pipe_lst, pipe_lst->command);
	else if (pipe_lst->to_ex == 1 && access(pipe_lst->cmd_pa, X_OK) == -1)
		error_cmd_not_executable(pipe_lst, pipe_lst->command);
	free_split(p);
	execve(pipe_lst->cmd_pa, pipe_lst->command, shell->envp);
}
