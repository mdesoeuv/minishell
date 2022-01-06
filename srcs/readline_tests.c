/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/06 13:01:54 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;

	change_directory("../test");
	line = readline("prompt? ");
	while (line && ft_strcmp(line, "exit") != 0)
	{
		add_history(line);
		printf("command entered = %s\n", line);
		if (ft_strcmp(line, "pwd") == 0)
			print_working_directory();
		free(line);
		line = readline("prompt? ");
	}
	free(line);
	return (0);
}
