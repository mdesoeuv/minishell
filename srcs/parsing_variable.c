/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:51:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/17 12:32:07 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_cmd_variable_change(int index_start, t_shell *shell,
	char *variable_result)
{
	char	*tmp;

	tmp = ft_strndup(shell->cmd_tmp, index_start - 1);
	if (!tmp)
		ft_free("Error : malloc error\n", shell, 1, 1);
	tmp = ft_strjoin(tmp, variable_result);
	if (!shell->cmd_tmp)
		ft_free("Error : malloc error\n", shell, 1, 1);
	shell->cmd_tmp = ft_strjoin(tmp, ft_substr(shell->cmd_tmp, index_start - 1,
				ft_strlen(shell->cmd_tmp) - index_start + 1));
	if (!shell->cmd_tmp)
		ft_free("Error : malloc error\n", shell, 1, 1);
	free(tmp);
}

void	ft_variable_replace(int i, t_shell *shell)
{
	int		index_start;
	char	*variable_name;
	char	*variable_result;

	index_start = i + 1;
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != ' '
		&& shell->cmd_tmp[i] != '\"')
		i++;
	variable_name = ft_substr(shell->cmd_tmp, index_start, i - index_start);
	if (!variable_name)
		ft_free("Error : malloc error\n", shell, 1, 1);
	if (ft_strcmp(variable_name, "?") == 0)
		variable_result = ft_strdup("0");
	else
		variable_result = ft_getenv(shell, variable_name);
	free(variable_name);
	ft_memmove(&(shell->cmd_tmp[index_start - 1]), &(shell->cmd_tmp[i]),
		ft_strlen(shell->cmd_tmp));
	if (variable_result)
		ft_cmd_variable_change(index_start, shell, variable_result);
}
