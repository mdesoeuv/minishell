/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/14 14:56:03 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_sig	g_sig;

void	ft_print_shell_struct(t_shell	shell)
{
	int		i;

	printf("nbr commands = %i \n", shell.cmd_nbr);
	printf("args_command =");
	i = -1;
	while (shell.list_start->command[++i])
		printf("%s|", shell.list_start->command[i]);
	printf("\n");
	// printf("chevron_nbr_in = %i\n", shell.list_start->chevron_nbr_in);
	// printf("chevron_nbr_out = %i\n", shell.list_start->chevron_nbr_out);
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
		// printf("chevron_nbr_in = %i\n", shell.list_start->chevron_nbr_in);
		// printf("chevron_nbr_out = %i\n", shell.list_start->chevron_nbr_out);
		if (shell.list_start->chevron_nbr_in != 0)
			printf("file_in =|%s|\n", shell.list_start->file_in);
		if (shell.list_start->chevron_nbr_out != 0)
			printf("file_out =|%s|\n", shell.list_start->file_out);
	}
}

int	copy_set_envp(t_shell *shell, char **envp)
{
	int		i;
	int		envp_size;
	char	**new_envp;

	envp_size = get_env_size(shell);
	new_envp = malloc(sizeof(char *) * (envp_size + 1));
	if (!new_envp)
		ft_exit(shell);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			ft_exit(shell);
		i++;
	}
	new_envp[i] = NULL;
	shell->envp = new_envp;
	return (1);
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
	copy_set_envp(&shell, envp);
	shell.return_val = 0;
	line = readline("\033[0;36m\033[1m minishell ▸ \033[0m");
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
			else if (ft_strcmp(shell.list_start->command[0], "export") == 0)
				ft_export(&shell, shell.list_start->command[1]);
			else if (ft_strcmp(shell.list_start->command[0], "unset") == 0)
				ft_unset(&shell, shell.list_start->command[1]);
			else if (ft_strcmp(shell.list_start->command[0], "env") == 0)
				ft_env(&shell, shell.list_start->command[1]);
			else if (ft_strcmp(shell.list_start->command[0], "exit") == 0)
			{
				is_exit = ft_exit(&shell);
				free(line);
				if (is_exit == 0)
					break ;
			}
			else
				cmd_process(&shell);
			shell.return_val = 0;
		}
		free(line);
		line = readline("\033[0;36m\033[1m minishell ▸ \033[0m");
	}
	free(line);
	return (0);
}
