/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/04 14:44:19 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_return_val;

static int	ft_incr_shell_env(t_shell *shell)
{
	char	*getenv;
	char	*shell_lvl;
	char	*final_res;

	getenv = ft_getenv(shell, "SHLVL");
	if (!getenv)
		getenv = "1";
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

static int	copy_set_envp(t_shell *shell, char **envp)
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

static void	ft_init_main(t_shell *shell, char ***argv, char ***envp)
{
	(void)(*argv);
	shell->is_exit = 1;
	g_return_val = 0;
	signal(SIGINT, &sig_int);
	signal(SIGQUIT, &sig_quit);
	shell->envp = (*envp);
	copy_set_envp(shell, (*envp));
	set_shell_path(shell);
	g_return_val = 0;
}

void static	ft_check_if_empty(t_shell shell)
{
	if (shell.cmd_nbr > 1)
	{
		while (shell.list_start)
		{
			if (shell.list_start->chevron_nbr_out > 0
				|| shell.list_start->chevron_nbr_in > 0
				|| shell.list_start->command[0])
				return ;
			shell.list_start = shell.list_start->next;
		}
		ft_putstr_fd("minishell :  syntax error unexpected token\n", 2);
		g_return_val = 258;
		return ;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;

	if (argc != 1)
		return (0);
	ft_init_main(&shell, &argv, &envp);
	shell.readline = readline("\001\033[0;36m\033[1m\002minishell > \001\033[0m\002");
	while (shell.readline)
	{
		add_history(shell.readline);
		if (ft_parsing(shell.readline, &shell, -1, 0) != -1
			&& shell.list_start->command)
		{
			new_cmd_process(&shell);
			signal(SIGINT, &sig_int);
		}
		ft_check_if_empty(shell);
		ft_free("", &shell, g_return_val, 0);
		shell.readline = \
			readline("\001\033[0;36m\033[1m\002minishell > \001\033[0m\002");
	}
	free(shell.readline);
	clear_history();
	return (g_return_val);
}
