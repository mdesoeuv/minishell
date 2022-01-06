/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:17 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/06 19:05:43 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*line;

	line = readline("prompt? ");
	while (line && ft_strcmp(line, "exit") != 0)
	{
		add_history(line);
		ft_parsing(line);
		printf("command entered = %s\n", line);
		if (ft_strcmp(line, "pwd") == 0)
			print_working_directory();
		if (ft_strncmp(line, "cd", 2) == 0)
			change_directory(line);
		free(line);
		line = readline("prompt? ");
	}
	free(line);
	return (0);
}
