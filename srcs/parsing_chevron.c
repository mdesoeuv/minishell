/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_chevron.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:35:13 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/20 10:39:02 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_file_in_out(t_shell *shell, int i)
{
	char	*file_name;
	int		index_start;

	index_start = i;
	dprintf(1, "hey");
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

static int	ft_new_pipe_chevron2_part2(t_shell	*shell,	\
	int i, t_list_pipes	*new_pipe, int index_start)
{		
	i = 0;
	if (new_pipe->chevron_nbr_out > 0)
	{
		if (new_pipe->chevron_nbr_out > 2)
			return (ft_free("Error : trop de chevrons > parse errror near >\n",
					shell, 1, 0));
		new_pipe->file_out = ft_file_in_out(shell, index_start);
		// creation
	}
	return (0);
}

static int	ft_new_pipe_chevron2(t_list_pipes	*new_pipe,
	t_shell	*shell)
{
	int	index_start;
	int i;

	index_start = 0;
	i = 0;
	while (shell->cmd_tmp[i++])
	{
		if (shell->cmd_tmp[i] == '\"')
		{
			i++;
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != '\"')
				i++;
		}
		else if (shell->cmd_tmp[i] == '\'')
		{
			i++;
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != '\'')
				i++;
		}
		else if (shell->cmd_tmp[i] == '>')
		{
			index_start = i;
			new_pipe->chevron_nbr_out = 1;
			ft_memmove(&(shell->cmd_tmp[i]), &shell->cmd_tmp[i + 1],
				ft_strlen(shell->cmd_tmp) - i);
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] == '>')
			{
				ft_memmove(&shell->cmd_tmp[i], &shell->cmd_tmp[i + 1],
					ft_strlen(shell->cmd_tmp) - i);
				new_pipe->chevron_nbr_out++;
			}
			ft_new_pipe_chevron2_part2(shell, i, new_pipe, index_start);
		}
		if (!shell->cmd_tmp[i])
			break ;
	}
	return (ft_new_pipe_name_args(new_pipe, shell));
}

static int	ft_new_pipe_chevron1_part2(t_shell	*shell,
	int i, t_list_pipes	*new_pipe, int index_start)
{
	i = 0;
	if (new_pipe->chevron_nbr_in > 0)
	{
		if (new_pipe->chevron_nbr_in > 2)
			return (ft_free("Error : trop de chevrons < parse errror near <\n",
					shell, 1, 0));
		new_pipe->file_in = ft_file_in_out(shell, index_start);
		// check if exist
	}
	return (0);
}

int	ft_new_pipe_chevron1(t_shell	*shell)
{
	int				index_start;
	int				i;
	t_list_pipes	*new_pipe;

	i = 0;
	new_pipe = ft_lstnew_pipes();
	if (!new_pipe)
		return (ft_free("Error : malloc error\n", shell, 1, 1));
	ft_lstadd_back_pipes(&(shell->list_start), new_pipe);
	index_start = 0;
	dprintf(1, "i=%i\n", i);
	while (shell->cmd_tmp[i])
	{
		dprintf(1, "i=%i\n", i);
		if (shell->cmd_tmp[i] == '\"')
		{
			i++;
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != '\"')
				i++;
		}
		else if (shell->cmd_tmp[i] == '\'')
		{
			i++;
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != '\'')
				i++;
		}
		else if (shell->cmd_tmp[i] == '<')
		{
			index_start = i;
			new_pipe->chevron_nbr_in = 1;
			ft_memmove(&(shell->cmd_tmp[i]), &shell->cmd_tmp[i + 1],
				ft_strlen(shell->cmd_tmp) - i);
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] == '<')
			{
				ft_memmove(&shell->cmd_tmp[i], &shell->cmd_tmp[i + 1],
					ft_strlen(shell->cmd_tmp) - i);
				new_pipe->chevron_nbr_in++;
			}
			ft_new_pipe_chevron1_part2(shell, i, new_pipe, index_start);
		}
		if (!shell->cmd_tmp[i])
		{
			dprintf(1, "hey");
			break ;
		}
		i++;
	}
	return (ft_new_pipe_chevron2(new_pipe, shell));
}
