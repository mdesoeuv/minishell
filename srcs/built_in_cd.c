/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 11:53:58 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/18 14:58:37 by mdesoeuv         ###   ########lyon.fr   */
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

char	*cd_tilde(t_shell *shell, char *path)
{
	char	*absolute_path;

	if (path[0] != '~')
		return (NULL);
	absolute_path = offset_dup(path, 1);
	absolute_path = ft_strjoin_free_s2(ft_getenv(shell, "HOME"), absolute_path);
	if (!absolute_path)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	return (absolute_path);
}

void	cd_print_error_msg(char *path)
{
	ft_putstr_fd("cd: no such file or directory: ", 2);
	ft_putendl_fd(path, 2);
}

int	change_directory(t_shell *shell, char *path)
{
	int		ret_value;
	char	*old_path;
	char	*curr_path;
	char	*tmp_path;

	old_path = return_working_directory();
	old_path = ft_strjoin_free_s2("OLDPWD=", old_path);
	if (!old_path)
		ft_free("memory allocation error\n", shell, 1, 1);
	ft_export(shell, old_path);
	tmp_path = cd_tilde(shell, path);
	if (tmp_path != NULL)
		path = tmp_path;
	ret_value = chdir(path);
	if (ret_value < 0)
		cd_print_error_msg(path);
	curr_path = return_working_directory();
	curr_path = ft_strjoin_free_s2("PWD=", curr_path);
	if (!old_path)
		ft_free("memory allocation error\n", shell, 1, 1);
	ft_export(shell, curr_path);
	free(curr_path);
	free(old_path);
	free(tmp_path);
	return (ret_value);
}
