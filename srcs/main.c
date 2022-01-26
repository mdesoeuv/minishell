/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/26 12:15:33 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int 	return_val;

int	ft_incr_shell_env(t_shell *shell)
{
	char	*getenv;
	char	*shell_lvl;
	char	*final_res;

	getenv = ft_getenv(shell, "SHLVL");
	if (!getenv)
		getenv = "1";
	ft_unset(shell, "SHLVL");
	shell_lvl = ft_itoa(ft_atoi(getenv) + 1);
	if (!shell_lvl)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	final_res = ft_strjoin_free_s2("SHLVL=", shell_lvl);
	if (!final_res)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	ft_export(shell, final_res);
	free(final_res);
	return (1);
}

int	copy_set_envp(t_shell *shell, char **envp)
{
	int		i;
	int		envp_size;
	char	**new_envp;

	envp_size = get_env_size(shell);
	new_envp = malloc(sizeof(char *) * (envp_size + 1));
	if (!new_envp)
		ft_free("Error : malloc error\n", shell, 1, 1);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			ft_free("Error : malloc error\n", shell, 1, 1);
		i++;
	}
	new_envp[i] = NULL;
	shell->envp = new_envp;
	ft_incr_shell_env(shell);
	return (1);
}

void	set_shell_path(t_shell *shell)
{
	char	*shell_path;

	shell_path = return_working_directory();
	shell_path = ft_strjoin_free_s2("SHELL=", shell_path);
	if (!shell_path)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	ft_export(shell, shell_path);
	free(shell_path);
}

int    main(int argc, char **argv, char **envp)
{
	t_shell		shell;

	(void)argc;
	(void)argv;
	shell.is_exit = 1;
	sig_init();
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	shell.envp = envp;
	copy_set_envp(&shell, envp);
	set_shell_path(&shell);
	shell.return_val = 0;
	shell.readline = readline("\033[0;36m\033[1m minishell ▸ \033[0m");
	while (shell.readline)
	{
		add_history(shell.readline);
		if (ft_parsing(shell.readline, &shell) != -1 && shell.list_start->command)
		{
			ft_print_shell_struct(shell);
			new_cmd_process(&shell);
		}
		ft_free("", &shell, shell.return_val, 0);
		shell.readline = readline("\033[0;36m\033[1m minishell ▸ \033[0m");
	}
	free(shell.readline);
	clear_history();
	return (shell.return_val);
}
