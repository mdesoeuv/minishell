/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_getenv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 15:53:51 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/14 16:10:33 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(t_shell *shell, char *var_name)
{
	int	index;
	int	i;

	index = is_value_in_envp(shell, var_name);
	if (index < 0)
		return (NULL);
	i = 0;
	while (shell->envp[index][i] && shell->envp[index][i] != '=')
		i++;
	i++;
	return (&(shell->envp[index][i]));
}
