/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/30 15:20:25 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc(t_group *group)
{
	char	*line;

	close(group->pipefd[0]);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
		{
			error_msg("incomplete here_doc");
			exit(1);
		}
		if (ft_strncmp(line, group->heredoc, ft_strlen(group->heredoc)) == 0)
		{
			free(line);
			exit(0);
		}
		ft_putstr_fd(line, group->pipefd[1]);
		free(line);
	}
	close(group->pipefd[1]);
}

int	handle_heredoc(t_group *group)
{
	pid_t	pid;

	// close or unlink pipefd if it is already used
	if (pipe(group->pipefd) == -1)
	{
		error_msg("heredoc pipe");
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		error_msg("heredoc fork");
		return (0);
	}
	if (pid == 0)
		write_heredoc(group);
	waitpid(pid, NULL, 0);
	close(group->pipefd[1]);
	group->original_stdin = dup(STDIN_FILENO);
	dup2(group->pipefd[0], STDIN_FILENO);
	close(group->pipefd[0]);
	return (1);
}

int redirect(t_group *group)
{
	if (!group->cmd[0])
		return (0);
	if (group->infile == -1)
	{
		error_msg(group->infile_name);
		return (0);
	}
	if (group->outfile == -1)
	{
		error_msg(group->outfile_name);
		return (0);
	}
	if (group->heredoc && !handle_heredoc(group))
		return (0);
	else if (group->infile != STDIN_FILENO)
	{
		group->original_stdin = dup(STDIN_FILENO);
		if (dup2(group->infile, STDIN_FILENO) == -1)
		{
			error_msg(group->infile_name);
			return (0);
		}
		if (close(group->infile) == -1)
		{
			error_msg(group->infile_name);
			return (0);
		}
	}
	if (group->outfile != STDOUT_FILENO)
	{	
		group->original_stdout = dup(STDOUT_FILENO);
		if (dup2(group->outfile, STDOUT_FILENO) == -1)
		{
			error_msg(group->outfile_name);
			return (0);
		}
		if (close(group->outfile) == -1)
		{
			error_msg(group->outfile_name);
			return (0);
		}
	}
	return (1);
}
