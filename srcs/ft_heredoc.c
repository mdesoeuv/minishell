/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesoeuv <mdesoeuv@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 13:22:11 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/02/09 17:49:33 by mdesoeuv         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_change_sig(void)
{
	signal(SIGINT, &heredoc_sig_int);
	signal(SIGQUIT, &sig_quit_heredoc);
}

void	ft_fork_error(t_shell *shell)
{
	ft_putstr_fd("minishell: fork: Resource temporarily unavailable\n", 2);
	shell->fork_error = 1;
	g_return_val = 1;
}

static int	heredoc_process(t_shell *shell, char *ending_line, int pipe_fd[2])
{
	char	*total_line;
	char	*line;

	total_line = ft_strdup("");
	if (!total_line)
		ft_free("minishell: memory allocation error\n", shell, 1, 1);
	close(pipe_fd[0]);
	ft_change_sig();
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
	exit(0);
}

int	here_doc_v2(t_shell *shell, t_list_pipes *pipe_lst)
{
	char	*ending_line;
	int		pipe_fd[2];
	pid_t	pid;
	int		child_status;

	if (shell->no_heredoc == 1)
		return (0);
	ending_line = pipe_lst->file_in;
	pipe(pipe_fd);
	pid = fork();
	if (pid < 0)
		ft_fork_error(shell);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (pid == 0)
		heredoc_process(shell, ending_line, pipe_fd);
	close(pipe_fd[1]);
	waitpid(pid, &child_status, 0);
	eval_child_status(child_status);
	if (g_return_val == 1)
		shell->no_heredoc = 1;
	return (pipe_fd[0]);
}
