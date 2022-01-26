/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 17:22:18 by vchevill          #+#    #+#             */
/*   Updated: 2022/01/26 14:02:26 by vchevill         ###   ########.fr       */
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
