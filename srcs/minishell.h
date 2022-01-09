/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vchevill <vchevill@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 13:00:41 by mdesoeuv          #+#    #+#             */
/*   Updated: 2022/01/09 01:24:35 by vchevill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <termios.h>
# include "../libft/libft.h"

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef struct s_cmd
{
	char	*cmd_path;
	char	**cmd_arg;
	char	*cmd_in;
	char	*cmd_out;
}	t_cmd;

typedef struct s_shell
{
	int					pipes_nbr;
	struct s_list_pipes	*list_start;
}	t_shell;

void	print_working_directory(void);
char	*return_working_directory(void);
int		change_directory(char *arg);
void	ft_parsing(char *line, t_shell	*shell);
void	ft_free(char *message, t_shell	*shell, int is_error);
#endif
