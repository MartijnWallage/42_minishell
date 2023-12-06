/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 10:30:04 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	child(t_group *group)
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

void	pipeline(t_group *group)
{
	t_group	*current;
	t_group	*last;
	int		status;

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