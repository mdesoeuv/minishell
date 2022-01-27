/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/14 10:49:14 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/27 11:50:06 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_shell_struct(t_shell	shell)
{
	int		i;

	printf("nbr commands = %i \n", shell.cmd_nbr);
	printf("args_command =");
	i = -1;
	while (shell.list_start->command[++i])
		printf("%s|", shell.list_start->command[i]);
	printf("\n");
	if (shell.list_start->chevron_nbr_in != 0)
		printf("file_in =|%s|\n", shell.list_start->file_in);
	if (shell.list_start->chevron_nbr_out != 0)
		printf("file_out =|%s|\n", shell.list_start->file_out);
	while (shell.list_start->next)
	{
		shell.list_start = shell.list_start->next;
		printf("args_command =");
		i = -1;
		while (shell.list_start->command[++i])
			printf("%s|", shell.list_start->command[i]);
		printf("\n");
		if (shell.list_start->chevron_nbr_in != 0)
			printf("file_in =|%s|\n", shell.list_start->file_in);
		if (shell.list_start->chevron_nbr_out != 0)
			printf("file_out =|%s|\n", shell.list_start->file_out);
	}
}

void	display_split(char *message, char **split)
{
	int	i;

	ft_putstr("Displaying ");
	ft_putendl(message);
	i = 0;
	while (split[i])
	{
		ft_putendl(split[i]);
		i++;
	}
	ft_putendl("--end");
}

void	print_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		ft_putendl(split[i]);
		i++;
	}
	ft_putendl("");
}
