/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 11:53:58 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/18 14:19:37 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	parsing cd cmd
*/

char	*offset_dup(char *s, size_t offset)
{
	size_t	len;
	size_t	i;
	char	*path;

	len = ft_strlen(s);
	path = malloc(sizeof(char) * (len - offset + 1));
	if (!path)
		return (NULL);
	i = 0;
	while (offset < len)
		path[i++] = s[offset++];
	path[i] = 0;
	return (path);
}

int	change_directory(t_shell *shell, char *path)
{
	int		ret_value;
	char	*old_path;
	char	*curr_path;

	old_path = return_working_directory();
	old_path = ft_strjoin_free_s2("OLDPWD=", old_path);
	if (!old_path)
		ft_free("minishell: cannot allocate memory\n", \
			shell, shell->return_val, 1);
	ft_export(shell, old_path);
	ret_value = chdir(path);
	if (ret_value < 0)
		perror("cd");
	curr_path = return_working_directory();
	curr_path = ft_strjoin_free_s2("PWD=", curr_path);
	if (!old_path)
		ft_free("minishell: cannot allocate memory\n", \
			shell, shell->return_val, 1);
	ft_export(shell, curr_path);
	free(curr_path);
	free(old_path);
	return (ret_value);
}
