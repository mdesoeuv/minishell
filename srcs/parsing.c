/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 19:06:14 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/08 18:00:25 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_new_pipe_name_args(char *command, t_list_pipes	*new_pipe)
{
	char	**cmd;

	cmd = ft_split_quotes(command, ' ');
	dprintf(1,"ddddd %i", new_pipe->chevron_nbr_in);
	new_pipe->command = cmd;
}

void	ft_file_in_out(char *command, t_list_pipes	*new_pipe,
		int chevron_nbr, int i)
{
	char	*file_name;
	int		index_start;

	dprintf(1,"nbr chevrons = %i\n", chevron_nbr);
	if (chevron_nbr == 0)
	{
		new_pipe->chevron_nbr_in = 0;
		return;
	}
	while (command[i] != ' ')
		i++;
	index_start = i;
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
		while (command[i] != ' ')
			i++;
	}
	file_name = ft_substr(command, index_start, i - index_start); // checker les variables d'env dans des guillemets
	ft_memmove(&command[i], &command[i + 1], i - index_start);
	if (chevron_nbr == 1)
	{
		new_pipe->file_in = file_name;
		new_pipe->chevron_nbr_in = 1;
	}
	else if (chevron_nbr == 2)
	{
		new_pipe->file_in = file_name;
		new_pipe->chevron_nbr_in = 2;
	}
	else if (chevron_nbr == 3)
	{
		new_pipe->file_in = file_name;
		new_pipe->chevron_nbr_in = 3;
	}
	else if (chevron_nbr > 3)
	{
		//parse errror near >
	}
	else if (chevron_nbr == -1)
	{
		new_pipe->file_out = file_name;
		new_pipe->chevron_nbr_out = 1;
	}
	else if (chevron_nbr == -2)
	{
		new_pipe->file_out = file_name;
		new_pipe->chevron_nbr_out = 2;
	}
	else if (chevron_nbr == -3)
	{
		new_pipe->file_out = file_name;
		new_pipe->chevron_nbr_out = 3;
	}
	else if (chevron_nbr < -3)
	{
		//parse errror near >
	}
}

void	ft_new_pipe_chevron2(char *command, t_list_pipes	*new_pipe)
{
	int				i;
	int				count_chevron;

	i = -1;
	count_chevron = 0;
	while (command[++i])
	{
		if (command[i] == '>')
		{
			count_chevron--;
			while (command[++i] && command[i] == '>')
				count_chevron--;
		}
	}
	while (command[++i])
		if (command[i] == '>')
			return; //parse errror near > exit
	ft_file_in_out(command, new_pipe, count_chevron, i);
	ft_new_pipe_name_args(command, new_pipe);
}

void	ft_new_pipe_chevron1(char *command, t_list_pipes	**list_pipe_start)
{
	int				i;
	int				count_chevron;
	t_list_pipes	*new_pipe;

	new_pipe = ft_lstnew_pipes();
	ft_lstadd_back_pipes(list_pipe_start, new_pipe);
	dprintf(1,"commande entiere = %s\n", command);
	i = -1;
	count_chevron = 0;
	while (command[++i])
	{
		if (command[i] == '<')
		{
			count_chevron++;
			while (command[++i] && command[i] == '<')
				count_chevron++;
		}
	}
	while (command[++i])
		if (command[i] == '<')
			return;//parse errror near < exit
	ft_file_in_out(command, new_pipe, count_chevron, i);
	ft_new_pipe_chevron2(command, new_pipe);
}

void	ft_parsing(char *line, t_shell	*shell)
{
	int				i;
	int				start;
	int				start_quote_index;

	i = -1;
	start = 0;
	shell->list_start = NULL;
	while (line[++i])
	{
		if (!line[i + 1])
			break ;
		if (line[i] == '\'')
		{
			start_quote_index = i;
			i++;
			while (line[++i])
			{
				if (line[i] == '\'' && (line[i + 1] || line[i + 1] == ' '))
					break ;
				else if (line[i] == '\'')
				{
					ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i);
					ft_memmove(&line[start_quote_index], &line[start_quote_index + 1], ft_strlen(line) - start_quote_index);
					break ;
				}
			}
		}
		if (line[i] == '\"')
		{
			start_quote_index = i;
			i++;
			while (line[++i])
			{
				if (line[i] == '\"' && (line[i + 1] || line[i + 1] == ' '))
					break ;
				else if (line[i] == '\"')
				{
					ft_memmove(&line[i], &line[i + 1], ft_strlen(line) - i);
					ft_memmove(&line[start_quote_index], &line[start_quote_index + 1], ft_strlen(line) - start_quote_index);
					break ;
				}
			}
		}
		if (line[i] == '|')
		{
			ft_new_pipe_chevron1(ft_substr(line, start, i - start + 1), &(shell->list_start));
			start = i;
		}
	}
	ft_new_pipe_chevron1(ft_substr(line, start, i - start + 1), &(shell->list_start));
	i = -1;
	while (shell->list_start->command[++i])
		printf("%s/", shell->list_start->command[i]);
	printf("\n");
}
