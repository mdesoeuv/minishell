/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/08 18:12:44 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_print_shell_struct(t_shell	*shell)
{
	int	i;

	i = -1;
	printf("nbr pipes = %i \n", shell->pipes_nbr);
	while (shell->list_start->next)
	{
		while (shell->list_start->command[++i])
			printf("%s/", shell->list_start->command[i]);
		printf("\n");
		printf("file_in %s", shell->list_start->file_in);
		printf("chevron_nbr_in %i", shell->list_start->chevron_nbr_in);
		printf("file_out %s", shell->list_start->file_out);
		printf("chevron_nbr_out %i", shell->list_start->chevron_nbr_out);
		shell->list_start = shell->list_start->next;
	}
}

int	main(void)
{
	char	*line;
	t_shell	shell;
	
	line = readline("prompt? ");
	while (line && ft_strcmp(line, "exit") != 0)
	{
		add_history(line);
		ft_parsing(line, &shell);
		ft_print_shell_struct(&shell);
		printf("command entered = %s\n", line);
		if (ft_strcmp(line, "pwd") == 0)
			print_working_directory();
		if (ft_strncmp(line, "cd", 2) == 0)
			change_directory(line);
		free(line);
		line = readline("prompt? ");
	}
	// free pipes
	free(line);
	return (0);
}
