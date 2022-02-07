/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/02/05 18:06:08 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  ", 2);
	write(2, "\b\b", 2);
	write(2, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_return_val = 1;
}

void	sig_int_fork(int code)
{
	(void)code;
	write(2, "\n", 1);
	rl_replace_line("", 0);
	g_return_val = 130;
}

void	heredoc_sig_int(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  ", 2);
	write(2, "\b\b", 2);
	printf("\n");
	g_return_val = 1;
	exit(g_return_val);
}

void	sig_quit(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  ", 2);
	write(2, "\b\b", 2);
}

void	sig_quit_heredoc(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  ", 2);
	write(2, "\b\b", 2);
}
