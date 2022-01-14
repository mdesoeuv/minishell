/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 09:42:01 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/14 10:38:01 by mdesoeuv         ###   ########lyon.fr   */
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
		ft_exit(shell);
	i = 0;
	while (shell->envp[i])
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	free(shell->envp);
	new_envp[i] = ft_strdup(s);
	if (!new_envp[i])
		ft_exit(shell);
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

int	ft_export(t_shell *shell, char *s)
{
	char	*search_ret;
	int		envp_index;

	if (!s || ft_isalpha(s[0]) == 0)
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	search_ret = ft_strchr(s, '=');
	if (search_ret == NULL || search_ret + 1 == NULL)
		return (0);
	ft_unset(shell, s);
	add_envp(shell, s);
	return (0);
}
