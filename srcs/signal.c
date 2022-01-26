/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/26 15:53:40 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int code)
{
	(void)code;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_return_val = 130;
}

void	sig_int_fork(int code)
{
	(void)code;
	printf("\n");
	rl_replace_line("", 0);
	g_return_val = 130;
}

void	heredoc_sig_int(int code)
{
	(void)code;
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
