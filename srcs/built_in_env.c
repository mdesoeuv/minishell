/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 14:45:42 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/28 09:53:37 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_env_size(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
		i++;
	return (i);
}

void	sort_env(t_shell *shell)
{
	int		i;
	int		j;
	int		env_size;
	char	*tmp;

	i = 0;
	env_size = get_env_size(shell);
	while (i < env_size - 1)
	{
		j = i + 1;
		while (j < env_size)
		{
			if (ft_strcmp(shell->envp[i], shell->envp[j]) > 0)
			{
				tmp = shell->envp[i];
				shell->envp[i] = shell->envp[j];
				shell->envp[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(t_shell *shell)
{
	int		i;
	int		j;

	i = 0;
	sort_env(shell);
	while (shell->envp[i])
	{
		ft_putstr("declare -x ");
		j = 0;
		while (shell->envp[i][j] && shell->envp[i][j] != '=')
			j++;
		write(1, shell->envp[i], j);
		write(1, "=\"", 2);
		ft_putstr(&(shell->envp[i][++j]));
		write(1, "\"\n", 2);
		i++;
	}
}

int	ft_env(t_shell *shell, t_list_pipes *pipe_lst, char *command)
{
	int		i;
	char	*search_ret;

	if (command != NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("env: ", 2);
		if (pipe_lst->command[1])
		{
			ft_putstr_fd(pipe_lst->command[1], 2);
		}
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	sort_env(shell);
	i = 0;
	while (shell->envp[i])
	{	
		search_ret = ft_strchr(shell->envp[i], '=');
		if (!(search_ret == NULL || search_ret + 1 == NULL))
			ft_putendl_fd(shell->envp[i], 1);
		i++;
	}
	return (0);
}
