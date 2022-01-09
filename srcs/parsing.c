/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/09 22:57:19 by vchevill         ###   ########.fr       */
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
	if (command[i] == '\'')
	{
		while (command[++i])
			if (command[i] == '\'')
				break ;
	}
	else if (command[i] == '\"')
	{
		while (command[++i])
			if (command[i] == '\"')
				break ;
	}
	else
	{
		while (command[i] && command[i] != ' ')
			i++;
	}
	/*dprintf(1,"char index_start = %i\n", index_start);
	dprintf(1,"char index_end = %i\n", i);	
	dprintf(1,"char index_start = %c\n", command[index_start]);
	dprintf(1,"char index_end = %c\n", command[i]);*/
	file_name = ft_substr(command, index_start, i - index_start);// checker les variables d'env dans des guillemets
	if (!file_name)
		ft_free("Error : malloc error\n", shell, 1);
	//dprintf(1,"file_name = %s\n", file_name);
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
		if (count_chevron < -3)
			ft_free("Error : trop de chevrons > parse errror near >\n", shell, 1);
		new_pipe->file_out = ft_file_in_out(command, shell, index_start);
		new_pipe->chevron_nbr_out = count_chevron * -1;
	}
	ft_new_pipe_name_args(command, new_pipe, shell);
}

void	ft_new_pipe_chevron1(char *command, t_shell	*shell)
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
	while (command[++i])
	{
		if (command[i] == '<')
		{
			index_start = i;
			count_chevron++;
			ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
			while (command[i] && command[i] == '<')
			{
				ft_memmove(&command[i], &command[i + 1], ft_strlen(command) - i);
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
		if (count_chevron > 3)
			ft_free("Error : trop de chevrons < parse errror near <\n", shell, 1);
		new_pipe->file_in = ft_file_in_out(command, shell, index_start);
		new_pipe->chevron_nbr_in = count_chevron;
	}
	ft_new_pipe_chevron2(command, new_pipe, shell);
}

void	ft_parsing(char *line, t_shell	*shell)
{
	int				i;
	int				start;
	int				start_quote_index;

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
		if (line[i] == '\'')
		{
			if (i == 0 || line[i - 1] == ' ')
			{
				start_quote_index = i;
				while (line[++i])
				{
					if (line[i] == '\'' && (!line[i + 1] || line[i + 1] == ' '))
						break ;
					else if (line[i] == '\'')
					{
						ft_memmove(&line[start_quote_index], &line[start_quote_index + 1], ft_strlen(line) - start_quote_index);
						i--;
						ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i);
						i--;
						break ;
					}
				}
			}
			else if (line[i - 1] != ' ')
			{
				ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i); 
				if (!line[i + 1])
					ft_free("Error : unclosed quote\n", shell, 1);
				while (line[++i])
				{
					if (line[i] == '\'')
					{
						ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i);
						break ;
					}
					else if (!line[i + 1])
						ft_free("Error : unclosed quote\n", shell, 1);
				}
			}
		}
		if (line[i] == '\"')
		{
			if (i == 0 || line[i - 1] == ' ')
			{
				start_quote_index = i;
				while (line[++i])
				{
					if (line[i] == '\"' && (!line[i + 1] || line[i + 1] == ' '))
						break ;
					else if (line[i] == '\"')
					{
						ft_memmove(&line[start_quote_index], &line[start_quote_index + 1], ft_strlen(line) - start_quote_index);
						i--;
						ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i);
						i--;
						break ;
					}
				}
			}
			else if (line[i - 1] != ' ')
			{
				ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i); 
				if (!line[i + 1])
					ft_free("Error : unclosed quote\n", shell, 1);
				while (line[++i])
				{
					if (line[i] == '\"')
					{
						ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i);
						break ;
					}
					else if (!line[i + 1])
						ft_free("Error : unclosed quote\n", shell, 1);
				}
			}
		}
		if (line[i] == '|')
		{
			shell->pipes_nbr++;
			ft_new_pipe_chevron1(ft_substr(line, start, i - start), shell);
			start = i + 1;
		}
	}
	ft_new_pipe_chevron1(ft_substr(line, start, i - start + 1), shell);
}
