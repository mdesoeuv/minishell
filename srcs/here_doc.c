/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:22:11 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/21 15:08:40 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	restore_std(t_shell *shell)
{
	shell->tmp_stdin = dup(0);
	// close(0);
	shell->tmp_stdout = dup(1);
	// close(1);
	dup2(shell->save_stdin, 0);
	close(shell->save_stdin);
	dup2(shell->save_stdout, 1);
	close(shell->save_stdout);
	return (0);
}

void	restore_prev_std(t_shell *shell)
{
	shell->save_stdin = dup(0);
	// close(0);
	shell->save_stdout = dup(1);
	// close(1);
	dup2(shell->tmp_stdin, 0);
	close(shell->tmp_stdin);
	dup2(shell->tmp_stdout, 1);
	close(shell->tmp_stdout);
}

int	here_doc(t_shell *shell, t_list_pipes *pipe_lst)
{
	char	*ending_line;
	int		fd;
	char	*line;
	char	*prompt;
	char	*total_line;

	ending_line = pipe_lst->file_in;
	restore_std(shell);
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
	if (!total_line)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	line = readline(prompt);
	while (line && ft_strcmp(line, ending_line) != 0)
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
	restore_prev_std(shell);
	return (1);
}
