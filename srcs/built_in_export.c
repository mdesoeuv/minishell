/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 09:42:01 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/27 09:18:20 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_envp(t_shell *shell, char *s)
{
	int		i;
	int		env_size;
	char	**new_envp;

	env_size = get_env_size(shell);
	new_envp = malloc(sizeof(char *) * (env_size + 2));
	if (!new_envp)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	free(shell->envp);
	new_envp[i] = ft_strdup(s);
	if (!new_envp[i])
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	i++;
	new_envp[i] = NULL;
	shell->envp = new_envp;
	return (0);
}

int	is_value_in_envp(t_shell *shell, char *s)
{
	int	i;
	int	name_len;

	name_len = 0;
	while (s[name_len] && s[name_len] != '=')
		name_len++;
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(s, shell->envp[i], name_len) == 0 && \
			(shell->envp[i][name_len] == 0 || shell->envp[i][name_len] == '='))
			return (i);
		i++;
	}
	return (-1);
}

int	ft_export(t_shell *shell, char *s)
{
	int		i;

	if (!s[0])
	{
		print_sorted_env(shell);
		return (0);
	}
	i = 1;
	if (!(ft_isalpha(s[0]) == 1 || s[0] == '_'))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	ft_unset(shell, s);
	add_envp(shell, s);
	sort_env(shell);
	return (0);
}

void	unset_add(t_shell *shell, char *command)
{
	ft_unset(shell, command);
	add_envp(shell, command);
}

int	ft_export_multi(t_shell *shell, char **command)
{
	int		i;
	int		g_return_val;

	g_return_val = 0;
	if (!command[1])
	{
		print_sorted_env(shell);
		return (0);
	}
	i = 1;
	while (command[i])
	{
		if (!(ft_isalpha(command[i][0]) == 1 || command[i][0] == '_'))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(command[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_return_val = 1;
		}
		else
			unset_add(shell, command[i]);
		i++;
	}
	return (g_return_val);
}
