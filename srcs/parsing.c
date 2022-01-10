/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/10 13:33:58 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_new_pipe_name_args(char *command, t_list_pipes *new_pipe, t_shell *shell)
{
	char	**cmd;

	cmd = ft_split_quotes(command, ' ', shell);
	if (!cmd)
		ft_free("Error : malloc error\n", shell, 1);
	new_pipe->command = cmd;
}

char	*ft_variable_replace(char *command, int i, t_shell *shell)
{
	int		index_start;
	char	*variable_name;
	char	*variable_result;
	char	*tmp;
	char	*new_command;

	index_start = i + 1;
	while (command[i] && command[i] != ' ' && command[i] != '\"')
		i++;
	variable_name = ft_substr(command, index_start, i - index_start);// checker les variables d'env dans des guillemets
	if (!variable_name)
		ft_free("Error : malloc error\n", shell, 1);
	variable_result = getenv(variable_name);
	free(variable_name);
	if (variable_result)
	{
		ft_memmove(&command[index_start - 1], &command[i],
			ft_strlen(command) - index_start - 1);
		tmp = ft_strndup(command, index_start - 1);
		if (!tmp)
			ft_free("Error : malloc error\n", shell, 1);
		new_command = ft_strjoin(tmp, variable_result);
		if (!new_command)
			ft_free("Error : malloc error\n", shell, 1);
		new_command = ft_strjoin(new_command, ft_substr(command, index_start - 1, ft_strlen(command) - index_start + 1));
		if (!new_command)
			ft_free("Error : malloc error\n", shell, 1);
		free(tmp);
		return (new_command);
	}
	ft_memmove(&command[index_start - 1], &command[i],
		ft_strlen(command) - index_start - 1);
	return (command);
}

int	ft_parse_quotes(int i, int index_start, char *command,
	char quote_type, t_shell *shell)// mettre shell en variable globale
{
	int	start_quote_index;

	if (i == index_start || command[i - 1] == ' ')
	{
		start_quote_index = i;
		while (command[++i])
		{
			if (quote_type == '\"' && command[i] == '$')
				command = ft_variable_replace(command, i, shell);
			if (command[i] == quote_type)
			{
				ft_memmove(&command[start_quote_index], &command[start_quote_index + 1], ft_strlen(command) - start_quote_index);
				i--;
				ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
				i--;
				break ;
			}
		}
	}
	else if (command[i - 1] != ' ')
	{
		ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i); 
		if (!command[i + 1])
			ft_free("Error : unclosed quote\n", shell, 1);
		while (command[++i])
		{
			if (command[i] == quote_type)
			{
				ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
				break ;
			}
			else if (!command[i + 1])
				ft_free("Error : unclosed quote\n", shell, 1);
		}
	}
	dprintf(1, "command = %s , %c,%i\n", command, command[i], i);
	return (i);
}

char	*ft_file_in_out(char *command, t_shell *shell, int i)
{
	char	*file_name;
	int		index_start;

	index_start = ++i;
	while (command[i] == ' ')
	{
		ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
		index_start = i;
	}
	while (command[i] != ' ' && command[i])
	{
		if (command[i] == '\'' || command[i] == '\"')
			i = ft_parse_quotes(i, index_start, command, command[i], shell);
		i++;
	}
	file_name = ft_substr(command, index_start, i - index_start);// checker les variables d'env dans des guillemets
	if (!file_name)
		ft_free("Error : malloc error\n", shell, 1);
	ft_memmove(&command[index_start], &command[i],
		ft_strlen(command) - index_start);
	return (file_name);
}

void	ft_new_pipe_chevron2(char *command, t_list_pipes *new_pipe, t_shell *shell)
{
	int				i;
	int				index_start;
	int				count_chevron;

	i = -1;
	count_chevron = 0;
	while (command[++i])
	{
		if (command[i] == '>')
		{
			count_chevron--;
			index_start = i;
			ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
			while (command[i] && command[i] == '>')
			{
				ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
				count_chevron--;
			}
			break ;
		}
	}
	while (command[++i])
		if (command[i] == '>')
			ft_free("Error : trop de chevrons > parse errror near >\n", shell, 1);
	new_pipe->chevron_nbr_out = count_chevron;
	if (count_chevron < 0)
	{
		if (count_chevron < -2)
			ft_free("Error : trop de chevrons > parse errror near >\n", shell, 1);
		new_pipe->file_out = ft_file_in_out(command, shell, index_start);
		new_pipe->chevron_nbr_out = count_chevron * -1;
	}
	ft_new_pipe_name_args(command, new_pipe, shell);
}

void	ft_new_pipe_chevron1(char **command, t_shell	*shell)
{
	int				i;
	int				index_start;
	int				count_chevron;
	t_list_pipes	*new_pipe;

	new_pipe = ft_lstnew_pipes();
	new_pipe->*command = NULL;
	ft_lstadd_back_pipes(&(shell->list_start), new_pipe);
	i = -1;
	count_chevron = 0;
	while (*command[++i])
	{
		if (*command[i] == '<')
		{
			index_start = i;
			count_chevron++;
			ft_memmove(command[i], command[i + 1], ft_strlen(*command) - i);
			while (*command[i] && *command[i] == '<')
			{
				ft_memmove(**command[i], *command[i + 1], ft_strlen(*command) - i);
				count_chevron++;
			}
			break;
		}
	}
	while (command[++i])
		if (command[i] == '<')
			ft_free("Error : trop de chevrons < parse errror near <\n", shell, 1);
	new_pipe->chevron_nbr_in = count_chevron;
	if (count_chevron > 0)
	{
		if (count_chevron > 2)
			ft_free("Error : trop de chevrons < parse errror near <\n", shell, 1);
		new_pipe->file_in = ft_file_in_out(command, shell, index_start);
		new_pipe->chevron_nbr_in = count_chevron;
	}
	ft_new_pipe_chevron2(command, new_pipe, shell);
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
			ft_new_pipe_chevron1(&ft_substr(line, start, i - start), shell);
			start = i + 1;
		}
	}
	ft_new_pipe_chevron1(&ft_substr(line, start, i - start + 1), shell);
}
