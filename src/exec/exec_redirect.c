/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 20:17:44 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void restore_redirection(t_group *group)
{
	if (group->infile != STDIN_FILENO && dup2(group->original_stdin, STDIN_FILENO) == -1)
		error_msg("could not restore redirection");
	if (group->outfile != STDOUT_FILENO && dup2(group->original_stdout, STDOUT_FILENO) == -1)
		error_msg("could not restore redirection");
}

static void	write_heredoc(t_group *group, char *eof, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	dup2(pipefd[1], STDOUT_FILENO);
	while (1)
	{
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

static int	handle_heredoc(t_group *group, char *eof)
{
	pid_t	pid;
	int		pipefd[2];
	int		status;

	if (group->infile != STDIN_FILENO)
		close(group->infile);
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
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (close(pipefd[0]), 0);
	close(pipefd[0]);
	group->infile = group->original_stdin;	// A really bad way to indicate that original stdin needs to be restored
	return (1);
}

int	open_infile(t_group *group, char *path)
{
	if (group->infile != STDIN_FILENO)
		close(group->infile);
	group->infile = open(path, O_RDONLY, 0444);
	if (group->infile == -1)
		return (0);
	if (dup2(group->infile, STDIN_FILENO) == -1)
		return (0);
	if (close(group->infile) == -1)
		return (0);
	return (1);
}

int	open_outfile(t_group *group, char *path, bool append)
{
	if (group->outfile != STDOUT_FILENO)
		close(group->outfile);
	if (append)
		group->outfile = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		group->outfile = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (group->outfile == -1)
		return (0);
	if (dup2(group->outfile, STDOUT_FILENO) == -1)
		return (0);
	if (close(group->outfile) == -1)
		return (0);
	return (1);
}

int redirect(t_group *group)
{
	int	i;

	i = -1;
	while (group->cmd[++i])
	{
		if (ft_strncmp(group->cmd[i], "<<", 3) == 0
			&& !handle_heredoc(group, group->cmd[i + 1]))
			return (error_msg("here_doc"));
		else if (ft_strncmp(group->cmd[i], "<", 2) == 0
			&& !open_infile(group, group->cmd[i + 1]))
			return (error_msg(group->cmd[i + 1]));
		else if (ft_strncmp(group->cmd[i], ">>", 3) == 0
			&& !open_outfile(group, group->cmd[i + 1], true))
			return (error_msg(group->cmd[i + 1]));
		else if (ft_strncmp(group->cmd[i], ">", 2) == 0
			&& !open_outfile(group, group->cmd[i + 1], false))
			return (error_msg(group->cmd[i + 1]));
	}
	return (1);
}
