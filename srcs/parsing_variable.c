/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:51:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/11 12:50:18 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_cmd_variable_change(int index_start, t_shell *shell,
	char *variable_result, int is_malloc)
{
	char	*tmp;
	char	*tmp2;

	tmp = ft_strndup(shell->cmd_tmp, index_start - 1);
	if (!tmp)
		ft_free("Error : malloc error\n", shell, 1, 1);
	tmp = ft_strjoin_free_s1(tmp, variable_result);
	if (!tmp)
		ft_free("Error : malloc error\n", shell, 1, 1);
	tmp2 = ft_strjoin_free_s2(tmp, ft_substr(shell->cmd_tmp, index_start - 1,
				ft_strlen(shell->cmd_tmp) - index_start + 1));
	if (!tmp2)
		ft_free("Error : malloc error\n", shell, 1, 1);
	if (shell->cmd_tmp)
	{
		free(shell->cmd_tmp);
		shell->cmd_tmp = NULL;
	}
	shell->cmd_tmp = tmp2;
	if (is_malloc == 1)
		free(variable_result);
	free(tmp);
}

int static	ft_itoa_chgvar(char **variable_result, t_shell *shell)
{
	*variable_result = ft_itoa(g_return_val);
	if (!(*variable_result))
		ft_free("Error : malloc error\n", shell, 1, 1);
	return (1);
}

void	ft_variable_replace(int i, t_shell *shell)
{
	int		index_start;
	char	*variable_name;
	char	*variable_result;
	int		is_malloc;

	index_start = i + 1;
	is_malloc = 0;
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != ' '
		&& shell->cmd_tmp[i] != '/' && shell->cmd_tmp[i] != '\"'
		&& shell->cmd_tmp[i] != '\'' && shell->cmd_tmp[i] != '=')
		i++;
	variable_name = ft_substr(shell->cmd_tmp, index_start, i - index_start);
	if (!variable_name)
		ft_free("Error : malloc error\n", shell, 1, 1);
	if (variable_name[0] == '\0')
		return ;
	if (ft_strcmp(variable_name, "?") == 0)
		is_malloc = ft_itoa_chgvar(&variable_result, shell);
	else
		variable_result = ft_getenv(shell, variable_name);
	free(variable_name);
	ft_memmove(&(shell->cmd_tmp[index_start - 1]), &(shell->cmd_tmp[i]),
		ft_strlen(shell->cmd_tmp) - (index_start - 1));
	if (variable_result)
		ft_cmd_variable_change(index_start, shell, variable_result, is_malloc);
}
