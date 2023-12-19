/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/19 17:29:00 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	*restore_redirection(t_group *group)
{
	if (group->infile != STDIN_FILENO
		&& dup2(group->original_stdin, STDIN_FILENO) == -1)
		error_msg("could not restore redirection");
	if (group->heredoc != STDIN_FILENO
		&& dup2(group->original_stdin, STDIN_FILENO) == -1)
		error_msg("could not restore redirection");
	if (group->outfile != STDOUT_FILENO
		&& dup2(group->original_stdout, STDOUT_FILENO) == -1)
		error_msg("could not restore redirection");
	group->infile = STDIN_FILENO;
	group->heredoc = STDIN_FILENO;
	group->outfile = STDOUT_FILENO;
	return (NULL);
}

int	open_infile(t_group *group, char *path)
{
	if (group->infile != STDIN_FILENO)
	{
		close(group->infile);
		dup2(group->original_stdin, STDIN_FILENO);
	}
	group->infile = open(path, O_RDONLY, 0444);
	if (group->infile == -1)
		return (group->infile = STDIN_FILENO, 0);
	if (ft_dup2(group, group->infile, STDIN_FILENO) == -1)
		return (0);
	if (close(group->infile) == -1)
		return (group->infile = STDIN_FILENO, 0);
	return (1);
}

int	open_outfile(t_group *group, char *path, bool append)
{
	if (group->outfile != STDOUT_FILENO)
	{
		close(group->outfile);
		dup2(group->original_stdout, STDOUT_FILENO);
	}
	if (append)
		group->outfile = open(path, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		group->outfile = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (group->outfile == -1)
		return (0);
	if (ft_dup2(group, group->outfile, STDOUT_FILENO) == -1)
		return (0);
	if (close(group->outfile) == -1)
		return (group->outfile = STDOUT_FILENO, 0);
	return (1);
}

int	redirect(t_group *group)
{
	int	i;

	i = -1;
	while (group->cmd[++i])
	{
		if (ft_strncmp(group->cmd[i], "<<", 3) == 0
			&& !open_heredoc(group))
			return (redirect_error(group, "heredoc"));
		else if (ft_strncmp(group->cmd[i], "<", 2) == 0
			&& !open_infile(group, group->cmd[i + 1]))
			return (redirect_error(group, group->cmd[i + 1]));
		else if (ft_strncmp(group->cmd[i], ">>", 3) == 0
			&& !open_outfile(group, group->cmd[i + 1], true))
			return (redirect_error(group, group->cmd[i + 1]));
		else if (ft_strncmp(group->cmd[i], ">", 2) == 0
			&& !open_outfile(group, group->cmd[i + 1], false))
			return (redirect_error(group, group->cmd[i + 1]));
	}
	return (1);
}
