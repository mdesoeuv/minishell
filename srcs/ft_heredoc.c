/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:22:11 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/04 15:02:36 by vchevill         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_prev_std(t_shell *shell)
{
	shell->save_stdin = dup(0);
	shell->save_stdout = dup(1);
	dup2(shell->tmp_stdin, 0);
	close(shell->tmp_stdin);
	dup2(shell->tmp_stdout, 1);
	close(shell->tmp_stdout);
}

static int	heredoc_process(t_shell *shell, char *ending_line, int pipe_fd[2])
{
	char	*total_line;
	char	*line;

	total_line = ft_strdup("");
	if (!total_line)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	close(pipe_fd[0]);
	signal(SIGINT, &heredoc_sig_int);
	line = readline("> ");
	while (line && ft_strcmp(line, ending_line) != 0)
	{
		total_line = ft_strjoin_free(total_line, line);
		if (!total_line)
			ft_free("minishell: memory allocation error\n", shell, 1, 1);
		total_line = ft_strjoin_free_s1(total_line, "\n");
		if (!total_line)
			ft_free("minishell: memory allocation error\n", shell, 1, 1);
		line = readline("> ");
	}
	free(line);
	write(pipe_fd[1], total_line, ft_strlen(total_line));
	free(total_line);
	close(pipe_fd[1]);
	restore_prev_std(shell);
	exit(0);
}

int	here_doc_v2(t_shell *shell, t_list_pipes *pipe_lst)
{
	char	*ending_line;
	int		pipe_fd[2];
	pid_t	pid;

	ending_line = pipe_lst->file_in;
	pipe(pipe_fd);
	pid = fork();
	if (pid < 0)
		ft_free("minishell: fork error\n", shell, 1, 1);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		heredoc_process(shell, ending_line, pipe_fd);
	}
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	return (pipe_fd[0]);
}