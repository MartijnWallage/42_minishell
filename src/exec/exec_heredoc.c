/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 10:27:22 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	write_heredoc(t_group *group, char *eof, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	ft_dup2(group, pipefd[1], STDOUT_FILENO);
	while (1)
	{
		write(group->original_stdout, "> ", 2);
		line = get_next_line(group->original_stdin);
		if (line == NULL)
		{
			close(pipefd[1]);
			error_msg("incomplete here_doc");
			cleanup_and_exit(group, 2);
		}
		if (ft_strncmp(line, eof, ft_strlen(eof)) == 0)
		{
			free(line);
			close(pipefd[1]);
			cleanup_and_exit(group, 2);
		}
		ft_putstr_fd(line, pipefd[1]);
		free(line);
	}
}

int	handle_heredoc(t_group *group, char *eof)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

	if (group->infile != STDIN_FILENO)
	{
		close(group->infile);
		ft_dup2(group, group->original_stdin, STDIN_FILENO);
	}
	if (pipe(pipefd) == -1)
		return (0);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), (0));
	if (pid == 0)
		write_heredoc(group, eof, pipefd);
	else
		close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		*group->exitcode = WEXITSTATUS(status);
	else
		return (close(pipefd[0]), 0);
	if (ft_dup2(group, pipefd[0], STDIN_FILENO) == -1)
		return (close(pipefd[0]), 0);
	close(pipefd[0]);
	group->infile = group->original_stdin;
	return (1);
}