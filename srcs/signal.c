/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/26 14:57:15 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int code)
{
	(void)code;
	dprintf(1, "sig int calssique\n");
	if (g_return_val != -1)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_return_val = 130;
}

void	heredoc_sig_int(int code)
{
	(void)code;
	ft_putchar_fd('\n', STDERR_FILENO);
	g_return_val = 1;
	exit(g_return_val);
}

void	eval_child_status(int child_status)
{
	if (WIFEXITED(child_status) != 0)
		g_return_val = WEXITSTATUS(child_status);
	else
	{
		if (WIFSIGNALED(child_status) != 0)
			g_return_val = WTERMSIG(child_status) + 128;
	}
}
