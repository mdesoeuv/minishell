/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 10:33:06 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/14 11:14:24 by mdesoeuv         ###   ########lyon.fr   */
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
		ft_exit(shell);
	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (i != envp_index)
			new_envp[i] = shell->envp[j++];
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

	// display_split("before unset", shell->envp);
	if (!s || ft_isalpha(s[0]) == 0)
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
	// display_split("after unset", shell->envp);
	return (0);
}
