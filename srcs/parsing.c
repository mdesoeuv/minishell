/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/10 17:07:22 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_new_pipe_name_args(t_list_pipes *new_pipe, t_shell *shell)
{
	char	**cmd_tab;

	cmd_tab = ft_split_quotes(' ', shell);
	if (!cmd_tab)
		ft_free("Error : malloc error\n", shell, 1);
	new_pipe->command = cmd_tab;
}

void	ft_variable_replace(int i, t_shell *shell)
{
	int		index_start;
	char	*variable_name;
	char	*variable_result;
	char	*tmp;

	index_start = i + 1;
	while (shell->cmd_tmp[i] && shell->cmd_tmp[i] != ' ' && shell->cmd_tmp[i] != '\"')
		i++;
	variable_name = ft_substr(shell->cmd_tmp, index_start, i - index_start);// checker les variables d'env dans des guillemets
	if (!variable_name)
		ft_free("Error : malloc error\n", shell, 1);
	variable_result = getenv(variable_name);
	free(variable_name);
	if (variable_result)
	{
		ft_memmove(&(shell->cmd_tmp[index_start - 1]), &(shell->cmd_tmp[i]),
			ft_strlen(shell->cmd_tmp) - index_start - 1);
		tmp = ft_strndup(shell->cmd_tmp, index_start - 1);
		if (!tmp)
			ft_free("Error : malloc error\n", shell, 1);
		tmp = ft_strjoin(tmp, variable_result);
		if (!shell->cmd_tmp)
			ft_free("Error : malloc error\n", shell, 1);
		shell->cmd_tmp = ft_strjoin(tmp, ft_substr(shell->cmd_tmp, index_start - 1, ft_strlen(shell->cmd_tmp) - index_start + 1));
		if (!shell->cmd_tmp)
			ft_free("Error : malloc error\n", shell, 1);
		free(tmp);
		return ;
	}
	ft_memmove(&(shell->cmd_tmp[index_start - 1]), &(shell->cmd_tmp[i]),
		ft_strlen(shell->cmd_tmp) - index_start - 1);
}

int	ft_parse_quotes(int i, int index_start,
	char quote_type, t_shell *shell)
{
	int	start_quote_index;

	if (i == index_start || shell->cmd_tmp[i - 1] == ' ')
	{
		start_quote_index = i;
		while (shell->cmd_tmp[++i])
		{
			if (quote_type == '\"' && shell->cmd_tmp[i] == '$')
				ft_variable_replace(i, shell);
			if (shell->cmd_tmp[i] == quote_type)
			{
				ft_memmove(&(shell->cmd_tmp[start_quote_index]), &(shell->cmd_tmp[start_quote_index + 1]), ft_strlen(shell->cmd_tmp) - start_quote_index);
				i--;
				ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i);
				i--;
				break ;
			}
		}
	}
	else if (shell->cmd_tmp[i - 1] != ' ')
	{
		ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i); 
		if (!shell->cmd_tmp[i + 1])
			ft_free("Error : unclosed quote\n", shell, 1);
		while (shell->cmd_tmp[++i])
		{
			if (shell->cmd_tmp[i] == quote_type)
			{
				ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i);
				break ;
			}
			else if (!shell->cmd_tmp[i + 1])
				ft_free("Error : unclosed quote\n", shell, 1);
		}
	}
	return (i);
}

char	*ft_file_in_out(t_shell *shell, int i)
{
	char	*file_name;
	int		index_start;

	index_start = ++i;
	while (shell->cmd_tmp[i] == ' ')
	{
		ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i);
		index_start = i;
	}
	while (shell->cmd_tmp[i] != ' ' && shell->cmd_tmp[i])
	{
		if (shell->cmd_tmp[i] == '\'' || shell->cmd_tmp[i] == '\"')
			i = ft_parse_quotes(i, index_start, shell->cmd_tmp[i], shell);
		i++;
	}
	file_name = ft_substr(shell->cmd_tmp, index_start, i - index_start);// checker les variables d'env dans des guillemets
	if (!file_name)
		ft_free("Error : malloc error\n", shell, 1);
	ft_memmove(&(shell->cmd_tmp[index_start]), &(shell->cmd_tmp[i]),
		ft_strlen(shell->cmd_tmp) - index_start);
	return (file_name);
}

void	ft_new_pipe_chevron2(t_list_pipes *new_pipe, t_shell *shell)
{
	int				i;
	int				index_start;
	int				count_chevron;

	i = -1;
	count_chevron = 0;
	while (shell->cmd_tmp[++i])
	{
		if (shell->cmd_tmp[i] == '>')
		{
			count_chevron--;
			index_start = i;
			ft_memmove(&(shell->cmd_tmp)[i], &(shell->cmd_tmp)[i + 1], ft_strlen(shell->cmd_tmp) - i);
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] == '>')
			{
				ft_memmove(&(shell->cmd_tmp[i]), &(shell->cmd_tmp[i + 1]), ft_strlen(shell->cmd_tmp) - i);
				count_chevron--;
			}
			break ;
		}
	}
	while (shell->cmd_tmp[++i])
		if (shell->cmd_tmp[i] == '>')
			ft_free("Error : trop de chevrons > parse errror near >\n", shell, 1);
	new_pipe->chevron_nbr_out = count_chevron;
	if (count_chevron < 0)
	{
		if (count_chevron < -2)
			ft_free("Error : trop de chevrons > parse errror near >\n", shell, 1);
		new_pipe->file_out = ft_file_in_out(shell, index_start);
		new_pipe->chevron_nbr_out = count_chevron * -1;
	}
	ft_new_pipe_name_args(new_pipe, shell);
}

void	ft_new_pipe_chevron1(t_shell	*shell)
{
	int				i;
	int				index_start;
	int				count_chevron;
	t_list_pipes	*new_pipe;

	new_pipe = ft_lstnew_pipes();
	new_pipe->command = NULL;
	ft_lstadd_back_pipes(&(shell->list_start), new_pipe);
	i = -1;
	count_chevron = 0;
	while (shell->cmd_tmp[++i])
	{
		if (shell->cmd_tmp[i] == '<')
		{
			index_start = i;
			count_chevron++;
			ft_memmove(&(shell->cmd_tmp[i]), &shell->cmd_tmp[i + 1], ft_strlen(shell->cmd_tmp) - i);
			while (shell->cmd_tmp[i] && shell->cmd_tmp[i] == '<')
			{
				ft_memmove(&shell->cmd_tmp[i], &shell->cmd_tmp[i + 1], ft_strlen(shell->cmd_tmp) - i);
				count_chevron++;
			}
			break;
		}
	}
	while (shell->cmd_tmp[++i])
		if (shell->cmd_tmp[i] == '<')
			ft_free("Error : trop de chevrons < parse errror near <\n", shell, 1);
	new_pipe->chevron_nbr_in = count_chevron;
	if (count_chevron > 0)
	{
		if (count_chevron > 2)
			ft_free("Error : trop de chevrons < parse errror near <\n", shell, 1);
		new_pipe->file_in = ft_file_in_out(shell, index_start);
		new_pipe->chevron_nbr_in = count_chevron;
	}
	ft_new_pipe_chevron2(new_pipe, shell);
}

/* ft_parsing parse les quotes et découpe en pipes*/
void	ft_parsing(char *line, t_shell	*shell)
{
	int				i;
	int				start;

	i = -1;
	start = 0;
	shell->list_start = NULL;
	shell->pipes_nbr = 1;
	while (line[i + 1])
	{
		if ((line[i] == '\'' && line[i + 1] == '\'' ) || (line[i] == '\"' && line[i + 1] == '\"' ))
		{
			ft_memmove(&line[i], &line[i + 2], ft_strlen(line) - i);
			i = i - 2;
		}
		i++;
	}
	i = -1;
	while (line[++i])
	{
		if (line[i] == '|')
		{
			shell->pipes_nbr++;
			shell->cmd_tmp = ft_substr(line, start, i - start);
			ft_new_pipe_chevron1(shell);
			start = i + 1;
		}
	}
	shell->cmd_tmp = ft_substr(line, start, i - start + 1);
	ft_new_pipe_chevron1(shell);
}
