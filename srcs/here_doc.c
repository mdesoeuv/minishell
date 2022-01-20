/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:22:11 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/20 15:21:51 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	here_doc(t_shell *shell, t_list_pipes *pipe_lst)
{
	char	*ending_line;
	int		fd;
	char	*line;
	char	*prompt;
	char	*total_line;

	ending_line = pipe_lst->file_in;
	fd = open("tmp/heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (shell->cmd_nbr > 1)
		prompt = "pipe heredoc> ";
	else
		prompt = "heredoc> ";
	total_line = ft_strdup("");
	line = readline(prompt);
	while (ft_strcmp(line, ending_line) != 0)
	{
		total_line = ft_strjoin_free(total_line, line);
		total_line = ft_strjoin_free_s1(total_line, "\n");
		if (!total_line)
			ft_free("minishell: memory allocation error\n", shell, 1, 1);
		line = readline(prompt);
	}
	free(line);
	write(fd, total_line, ft_strlen(total_line));
	close(fd);
	free(total_line);
	return (1);
}
