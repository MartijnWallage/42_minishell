/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/24 18:34:28 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	write_heredoc(t_group *group, char *eof, int pipefd[2])
{
	char	*line;

	if (is_quotation_mark(*eof))
	{
		eof++;
		eof[ft_strlen(eof) - 1] = 0;
	}
	close(pipefd[0]);
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strncmp(line, eof, ft_strlen(eof)) == 0)
		{
			if (line == NULL)
				error_msg("warning: here-document delimited by end-of-file");
			else
				free(line);
			close(pipefd[1]);
			cleanup_and_exit(group, 0);
		}
		ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
}

static int	heredoc_waitpid(t_group *group, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		*group->exitcode = WEXITSTATUS(status);
		if (*group->exitcode)
			return (0);
	}
	return (1);
}

static int	handle_heredoc(t_group *group, char *eof)
{
	pid_t	pid;
	int		pipefd[2];

	if (group->infile != STDIN_FILENO)
		close(group->infile);
	if (pipe(pipefd) == -1)
		return (0);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), 0);
	if (pid == 0)
	{
		handle_signals(HEREDOC);
		write_heredoc(group, eof, pipefd);
	}
	else
		close(pipefd[1]);
	if (!heredoc_waitpid(group, pid))
		return (0);
	if (group->heredoc != STDIN_FILENO)
		close(group->heredoc);
	group->heredoc = pipefd[0];
	return (1);
}

int	parse_heredoc_group(t_group *group)
{
	int		i;

	if (!group->cmd || !group->cmd[0])
		return (1);
	i = -1;
	while (group->cmd[++i])
	{
		if (group->cmd[i][0] == '<' && group->cmd[i][1] == '<'
			&& !handle_heredoc(group, group->cmd[i + 1]))
			return (0);
	}
	return (1);
}

int	parse_heredoc(t_group *group)
{
	if (parse_heredoc_group(group) == 0)
		return (0);
	if (group->subshell && !parse_heredoc(group->subshell))
		return (0);
	if (group->next && !parse_heredoc(group->next))
		return (0);
	return (1);
}
