/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_quit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 10:51:03 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/09 10:59:59 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_quit(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  ", 2);
	write(2, "\b\b", 2);
}

void	sig_quit_fork(int code)
{
	(void)code;
	ft_putstr_fd("Quit: 3\n", 2);
	rl_on_new_line();
	rl_redisplay();
	g_return_val = 131;
}

void	sig_quit_heredoc(int code)
{
	(void)code;
	rl_on_new_line();
	rl_redisplay();
	write(2, "  ", 2);
	write(2, "\b\b", 2);
}
