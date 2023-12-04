/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:58 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 23:59:05 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void restore_redirection(t_group *group)
{
	if (group->heredoc && dup2(group->original_stdin, STDIN_FILENO) == -1)
		error_msg("could not restore redirection");
	if (group->infile != STDIN_FILENO && dup2(group->original_stdin, STDIN_FILENO) == -1)
		error_msg("could not restore redirection");
	if (group->outfile != STDOUT_FILENO && dup2(group->original_stdout, STDOUT_FILENO) == -1)
		error_msg("could not restore redirection");
}

void	simple_command(t_group *group)
{
	int		status;

	if (!group->cmd || !group->cmd[0])
		return ;
	if (!redirect(group))
	{
		if (group->operator == PIPE)
			cleanup_and_exit(group, 1);
		else
			return ;
	}
	if (is_builtin(group->cmd[0]))
		builtin(group);
	else if (group->operator == PIPE)
		exec(group);
	else
	{
		group->pid = fork();
		if (group->pid == 0)
			exec(group);
		waitpid(group->pid, &status, 0);
		if (WIFEXITED(status))
			group->exitcode = WEXITSTATUS(status);
		else
			error_msg("program quit unexpectedly");
	}
	restore_redirection(group);
}

static void	child(t_group *group)
{
	if (group->next && pipe(group->pipefd) == -1)
	{
		error_msg("");
		return ;
	}
	group->pid = fork();
	if (group->pid == -1)
	{
		error_msg("");
		return ;
	}
	if (group->pid == 0)
	{
		if (group->previous && group->infile == STDIN_FILENO)
		{
			dup2(group->previous->pipefd[0], STDIN_FILENO);
			close(group->previous->pipefd[0]);
		}
		if (group->next && group->outfile == STDOUT_FILENO)
		{
			close(group->pipefd[0]);
			dup2(group->pipefd[1], STDOUT_FILENO);
			close(group->pipefd[1]);
		}
		if (group->cmd && group->cmd[0])
			simple_command(group);
		else
			cleanup_and_exit(group, 0);
	}
	if (group->previous)
		close(group->previous->pipefd[0]);
	if (group->next)
		close(group->pipefd[1]);
}

void	executor(t_group *group)
{
	t_group	*current;
	t_group	*last;
	int		status;

	if (group->operator == 0)
	{
		simple_command(group);
		return ;
	}
	current = group;
	while (current)
	{
		child(current);
		current = current->next;
	}
	current = group;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		last = current;
		current = current->next;
	}
	if (WIFEXITED(status))
		last->exitcode = WEXITSTATUS(status);
	else
		error_msg(last->cmd[0]);
}
