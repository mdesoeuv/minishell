/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_chevron.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:35:13 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/02 16:52:31 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_file_in_out(t_shell *shell, int i)
{
	char	*file_name;
	int		index_start;

	index_start = i;
	while (shell->cmd_tmp[i] == ' ')
	{
		ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]),
			ft_strlen(shell->cmd_tmp) - i);
		index_start = i;
	}
	while (shell->cmd_tmp[i] != ' ' && shell->cmd_tmp[i])
	{
		if (shell->cmd_tmp[i] == '$')
			ft_variable_replace(i, shell);
		if (shell->cmd_tmp[i] == '\'' || shell->cmd_tmp[i] == '\"')
			i = ft_parse_quotes(i, index_start, shell->cmd_tmp[i], shell);
		i++;
	}
	file_name = ft_substr(shell->cmd_tmp, index_start, i - index_start);
	if (!file_name)
		ft_free("Error : malloc error\n", shell, 1, 1);	
	ft_memmove(&(shell->cmd_tmp[index_start]), &(shell->cmd_tmp[i]),
		ft_strlen(shell->cmd_tmp) - index_start);
	return (file_name);
}

static int	is_possible_create(t_shell *shell, t_list_pipes *pipe)
{
	if (shell->no_such_file == 1)
		return (0);
	if (access(pipe->file_out, F_OK) == -1)
		return (1);
	if (access(pipe->file_out, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(pipe->file_out, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		shell->no_such_file = 1;
		return (0);
	}
	if (is_directory(pipe, pipe->file_out) == 1)
	{
		shell->no_such_file = 1;
		return (0);
	}	
	return (1);
}

static int	ft_new_pipe_chevron2_part2(t_shell	*shell,	\
	t_list_pipes	*new_pipe, int i)
{		
	new_pipe->chevron_nbr_out = 1;
	ft_memmove(&(shell->cmd_tmp[i]), &shell->cmd_tmp[i + 1],
		ft_strlen(shell->cmd_tmp) - i);
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] == '>')
	{
		ft_memmove(&shell->cmd_tmp[i], &shell->cmd_tmp[i + 1],
			ft_strlen(shell->cmd_tmp) - i);
		new_pipe->chevron_nbr_out++;
	}
	if (new_pipe->chevron_nbr_out > 0)
	{
		if (new_pipe->chevron_nbr_out > 2)
			return (ft_free("minishell: syntax error near unexpected token >\n",
					shell, 258, 0));
		free(new_pipe->file_out);
		new_pipe->file_out = ft_file_in_out(shell, i);
		if (ft_strcmp(new_pipe->file_out, "") == 0)
			return (ft_putstr_fd_shell(
					"minishell: syntax error unexpected token \n", 2));
		if (is_possible_create(shell, new_pipe) == 1)
			ft_create_file(new_pipe->chevron_nbr_out, new_pipe->file_out);
		else
			new_pipe->to_execute = 0;
	}
	return (0);
}

static int	ft_new_pipe_chevron1_part2(t_shell	*shell,
	t_list_pipes	*new_pipe, int i)
{
	if (new_pipe->chevron_nbr_in > 1)
		close (here_doc_v2(shell, new_pipe));
	new_pipe->chevron_nbr_in = 1;
	ft_memmove(&(shell->cmd_tmp[i]), &shell->cmd_tmp[i + 1],
		ft_strlen(shell->cmd_tmp) - i);
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] == '<')
	{
		ft_memmove(&shell->cmd_tmp[i], &shell->cmd_tmp[i + 1],
			ft_strlen(shell->cmd_tmp) - i);
		new_pipe->chevron_nbr_in++;
	}
	if (new_pipe->chevron_nbr_in > 0)
	{
		if (new_pipe->chevron_nbr_in > 2)
			return (ft_free("minishell: syntax error near unexpected token <\n",
					shell, 258, 0));
		free(new_pipe->file_in);
		new_pipe->file_in = ft_file_in_out(shell, i);
		if (ft_strcmp(new_pipe->file_in, "") == 0)
			return (ft_putstr_fd_shell(
					"minishell: syntax error unexpected token \n", 2));
		if (new_pipe->chevron_nbr_in == 1)
			ft_check_if_file_exists(new_pipe, shell);
	}
	return (0);
}

int	ft_new_pipe_chevron(t_shell	*shell)
{
	int				i;
	t_list_pipes	*new_pipe;

	i = -1;
	new_pipe = ft_lstnew_pipes();
	if (!new_pipe)
		return (ft_free("Error : malloc error\n", shell, 1, 1));
	ft_lstadd_back_pipes(&(shell->list_start), new_pipe);
	while (shell->cmd_tmp[++i])
	{
		i = ft_dodge_quotes(shell->cmd_tmp, i);
		if (shell->cmd_tmp[i] == '<')
		{	
			if (ft_new_pipe_chevron1_part2(shell, new_pipe, i) == -1)
				return (-1);
		}
		else if (shell->cmd_tmp[i] == '>')
		{
			if (ft_new_pipe_chevron2_part2(shell, new_pipe, i) == -1)
				return (-1);
		}
		if (!shell->cmd_tmp[i])
			break ;
	}
	return (ft_new_pipe_name_args(new_pipe, shell));
}
