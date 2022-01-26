/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/26 09:58:34 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int code)
{
	(void)code;
	if (g_sig.pid == 0)
	{
		rl_replace_line("", 0);
		ft_putstr_fd("\n", 1);
		ft_putstr_fd("\033[0;36m\033[1m minishell ▸ \033[0m", 1);
		rl_on_new_line();
		rl_redisplay();
	}
	g_sig.exit_status = 130;
	g_sig.sigint = 1;
}

void	sig_quit(int code)
{
	ft_putstr("  \b\b");
	(void)code;
	return ;
}

void	sig_init(void)
{
	g_sig.sigint = 0;
	g_sig.pid = 0;
	g_sig.exit_status = 0;
}


void	eval_child_status(int child_status)
{
	if (WIFEXITED(child_status) != 0)
		g_sig.exit_status = WEXITSTATUS(child_status);
	else
	{
		if (WIFSIGNALED(child_status) != 0)
			g_sig.exit_status = WTERMSIG(child_status) + 128;
	}
}
