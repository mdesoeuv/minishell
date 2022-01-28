/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 11:53:58 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/28 11:28:19 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*offset_dup(char *s, size_t offset)
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

static char	*cd_tilde(t_shell *shell, char *path)
{
	char	*absolute_path;
	char	*no_arg_path;

	no_arg_path = NULL;
	if (!path)
	{
		no_arg_path = ft_strdup("~");
		if (!no_arg_path)
			ft_free("minishell: memory allocation error\n", shell, 1, 1);
		path = no_arg_path;
	}
	if (path[0] != '~')
		return (NULL);
	absolute_path = offset_dup(path, 1);
	absolute_path = ft_strjoin_free_s2(ft_getenv(shell, "HOME"), absolute_path);
	if (!absolute_path)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	free(no_arg_path);
	if (ft_getenv(shell, "HOME") == NULL)
		absolute_path[0] = 0;
	return (absolute_path);
}

static int	access_check(char *path)
{
	DIR	*dirptr;

	if (path[0] == 0)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (-1);
	}
	if (access(path, F_OK) == -1)
		return (cd_error_msg("No such file or directory", path));
	if (access(path, R_OK) == -1)
		return (cd_error_msg("Permission denied", path));
	dirptr = opendir(path);
	if (!dirptr)
		return (cd_error_msg("Not a directory", path));
	closedir(dirptr);
	return (1);
}

static void	set_old_path(t_shell *shell, char *tmp_path)
{
	char	*old_path;

	old_path = return_working_directory();
	if (!old_path)
	{
		free(tmp_path);
		ft_free("memory allocation error\n", shell, 1, 1);
	}
	old_path = ft_strjoin_free_s2("OLDPWD=", old_path);
	if (!old_path)
	{
		free(tmp_path);
		ft_free("memory allocation error\n", shell, 1, 1);
	}
	ft_export(shell, old_path);
	free(old_path);
}

int	change_directory(t_shell *shell, char *path)
{
	int		ret_value;
	char	*curr_path;
	char	*tmp_path;

	tmp_path = cd_tilde(shell, path);
	if (tmp_path != NULL)
		path = tmp_path;
	if (access_check(path) == -1)
	{
		free(tmp_path);
		return (1);
	}
	set_old_path(shell, tmp_path);
	ret_value = chdir(path);
	curr_path = return_working_directory();
	curr_path = ft_strjoin_free_s2("PWD=", curr_path);
	if (!curr_path)
	{
		free(tmp_path);
		ft_free("memory allocation error\n", shell, 1, 1);
	}
	ft_export(shell, curr_path);
	free(curr_path);
	free(tmp_path);
	return (ret_value);
}
