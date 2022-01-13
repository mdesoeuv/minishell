/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/13 14:04:22 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

void	ft_print_shell_struct(t_shell	shell)
{
	int		i;

	printf("nbr pipes = %i \n", shell.cmd_nbr);
	printf("args_command =");
	i = -1;
	while (shell.list_start->command[++i])
		printf("%s|", shell.list_start->command[i]);
	printf("\n");
	printf("chevron_nbr_in = %i\n", shell.list_start->chevron_nbr_in);
	printf("chevron_nbr_out = %i\n", shell.list_start->chevron_nbr_out);
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
		printf("chevron_nbr_in = %i\n", shell.list_start->chevron_nbr_in);
		printf("chevron_nbr_out = %i\n", shell.list_start->chevron_nbr_out);
		if (shell.list_start->chevron_nbr_in != 0)
			printf("file_in =|%s|\n", shell.list_start->file_in);
		if (shell.list_start->chevron_nbr_out != 0)
			printf("file_out =|%s|\n", shell.list_start->file_out);
	}
}

int    main(int argc, char **argv, char **envp)
{
	char		*line;
	t_shell		shell;
	int			is_exit;

	(void)argc;
	(void)argv;
	is_exit = 1;
	sig_init();
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	shell.envp = envp;
	shell.return_val = 0;
	line = readline("minishell: ");
	while (line)
	{
		add_history(line);
		ft_parsing(line, &shell);
		if (shell.list_start->command && shell.list_start->command[0])
		{
			ft_print_shell_struct(shell);
			if (ft_strcmp(shell.list_start->command[0], "pwd") == 0)
				print_working_directory();
			else if (ft_strcmp(shell.list_start->command[0], "cd") == 0)
				change_directory(line);
			else if (ft_strcmp(shell.list_start->command[0], "echo") == 0)
				ft_echo(&shell);
			else if (ft_strcmp(shell.list_start->command[0], "exit") == 0)
			{
				is_exit = ft_exit(&shell);
				if (is_exit == 0)
					break ;
			}
			else
				cmd_process(&shell);
			shell.return_val = 0;
		}
		free(line);
		line = readline("minishell: ");
	}
	free(line);
	return (0);
}
