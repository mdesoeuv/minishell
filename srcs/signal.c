/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/26 13:51:59 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_int(int code)
{
	(void)code;
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("\033[0;36m\033[1m minishell ▸ \033[0m", 1);
	// rl_replace_line("", 0);
	return_val = 130;
}

void	sig_quit(int code)
{
	ft_putstr("  \b\b");
	(void)code;
	return ;
}

void	sig_init(void)
{
	return_val = 0;
}


void	eval_child_status(int child_status)
{
	if (WIFEXITED(child_status) != 0)
		return_val = WEXITSTATUS(child_status);
	else
	{
		if (WIFSIGNALED(child_status) != 0)
			return_val = WTERMSIG(child_status) + 128;
	}
}
