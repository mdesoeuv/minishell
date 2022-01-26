/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 10:33:06 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/26 11:28:33 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	remove_envp(t_shell *shell, int envp_index)
{
	int		i;
	int		j;
	int		env_size;
	char	**new_envp;

	env_size = get_env_size(shell);
	new_envp = malloc(sizeof(char *) * (env_size));
	if (!new_envp)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (i != envp_index)
			new_envp[j++] = shell->envp[i];
		else
			free(shell->envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

int	ft_unset(t_shell *shell, char *s)
{
	int	envp_index;

	if (!s || !(ft_isalpha(s[0]) == 1 || s[0] == '_'))
	{
		ft_putstr_fd("minishell: unset: ", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd(": invalid parameter name\n", 2);
		return (1);
	}
	envp_index = is_value_in_envp(shell, s);
	if (envp_index < 0)
		return (0);
	remove_envp(shell, envp_index);
	return (0);
}

int	ft_unset_multi(t_shell *shell, char **command)
{
	int	i;
	int	ret_value;

	if (!command[1])
		return (0);
	i = 1;
	ret_value = 0;
	while (command[i])
	{
		if (ft_unset(shell, command[i]) == 1)
			ret_value = 1;
		i++;
	}
	return (ret_value);
}
