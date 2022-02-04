/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/04 15:41:07 by vchevill         ###   ########lyon.fr   */
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
	g_return_val = 1;
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
	printf("\n");
	g_return_val = 1;
	exit(g_return_val);
}

void	sig_quit(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
}

void	sig_quit_heredoc(int code)
{
	(void)code;
	rl_on_new_line();
}
