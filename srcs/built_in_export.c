/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 09:42:01 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/26 10:56:00 by mdesoeuv         ###   ########lyon.fr   */
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
		if (ft_strncmp(s, shell->envp[i], name_len) == 0)
			return (i);
		i++;
	}
	return (-1);
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

int	ft_export(t_shell *shell, char *s)
{
	char	*search_ret;
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

int	ft_export_multi(t_shell *shell, char **command)
{
	char	*search_ret;
	int		i;
	int		return_val;

	return_val = 0;
	dprintf(2, "cmd 0 = %s\n", command[0]);
	dprintf(2, "cmd 1 = %s\n", command[1]);
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
			return_val = 1;
		}
		else
		{
			search_ret = ft_strchr(command[i], '='); //
			ft_unset(shell, command[i]);
			add_envp(shell, command[i]);
		}
		i++;
	}
	sort_env(shell);
	return (return_val);
}
