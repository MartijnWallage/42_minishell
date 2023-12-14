/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/14 18:32:11 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	write_heredoc(t_group *group, char *eof, int pipefd[2])
{
	char	*line;

	handle_signals(HEREDOC);
	close(pipefd[0]);
	ft_dup2(group, pipefd[1], STDOUT_FILENO);
	while (1)
	{
		write(group->original_stdout, "> ", 2);
		line = get_next_line(group->original_stdin);
		if (line == NULL)
		{
			close(pipefd[1]);
			write(2, "\n", 1);
			error_msg("warning: here-document delimited by end-of-file");
			cleanup_and_exit(group, 0);
		}
		if (ft_strncmp(line, eof, ft_strlen(eof)) == 0)
		{
			free(line);
			close(pipefd[1]);
			cleanup_and_exit(group, 0);
		}
		ft_putstr_fd(line, pipefd[1]);
		free(line);
	}
}

void	close_previous_infile(t_group *group)
{
	if (group->infile != STDIN_FILENO)
	{
		close(group->infile);
		ft_dup2(group, group->original_stdin, STDIN_FILENO);
	}
}

int	handle_heredoc(t_group *group, char *eof)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

	close_previous_infile(group);
	if (pipe(pipefd) == -1)
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (pid == 0)
		write_heredoc(group, eof, pipefd);
	else
		close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		*group->exitcode = WEXITSTATUS(status);
		if (*group->exitcode)
			return (*group->exitcode);
	}
	if (ft_dup2(group, pipefd[0], STDIN_FILENO) == -1)
		return (close(pipefd[0]), 1);
	close(pipefd[0]);
	group->infile = group->original_stdin;
	return (0);
}
