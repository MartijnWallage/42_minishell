/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 23:58:09 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	write_heredoc(t_group *group)
{
	char	*line;

	close(group->heredoc_pipefd[0]);
	while (1)
	{
		line = get_next_line(group->original_stdin);
		if (line == NULL)
		{
			close(group->heredoc_pipefd[1]);
			error_msg("incomplete here_doc");
			cleanup_and_exit(group, 1);
		}
		if (ft_strncmp(line, group->heredoc, ft_strlen(group->heredoc)) == 0)
		{
			free(line);
			close(group->heredoc_pipefd[1]);
			cleanup_and_exit(group, 1);
		}
		ft_putstr_fd(line, group->heredoc_pipefd[1]);
		free(line);
	}
}

int	handle_heredoc(t_group *group)
{
	pid_t	pid;

	if (pipe(group->heredoc_pipefd) == -1)
		return (error_msg("heredoc pipe"));
	pid = fork();
	if (pid == -1)
		return (error_msg("heredoc fork"));
	if (pid == 0)
		write_heredoc(group);
	waitpid(pid, NULL, 0);
	close(group->heredoc_pipefd[1]);
	dup2(group->heredoc_pipefd[0], STDIN_FILENO);
	close(group->heredoc_pipefd[0]);
	return (1);
}

int redirect(t_group *group)
{
	if (!group->cmd || !group->cmd[0])
		return (0);
	if (group->infile == -1)
		return (error_msg(group->infile_name));
	if (group->outfile == -1)
		return (error_msg(group->outfile_name));
	if (group->heredoc && !handle_heredoc(group))
		return (0);
	if (group->infile != STDIN_FILENO)
	{
		if (dup2(group->infile, STDIN_FILENO) == -1)
			return (error_msg(group->infile_name));
		if (close(group->infile) == -1)
			return (error_msg(group->infile_name));
	}
	if (group->outfile != STDOUT_FILENO)
	{	
		if (dup2(group->outfile, STDOUT_FILENO) == -1)
			return (error_msg(group->outfile_name));
		if (close(group->outfile) == -1)
			return (error_msg(group->outfile_name));
	}
	return (1);
}
