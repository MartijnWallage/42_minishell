/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 10:10:36 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	dup_and_close(t_group *group)
{
	if (group->previous && group->previous->previous)
	{
		ft_dup2(group, group->previous->pipefd[0], STDIN_FILENO);
		close(group->previous->pipefd[0]);
	}
	if (group->next && group->next->operator == PIPE)
	{
		close(group->next->pipefd[0]);
		ft_dup2(group, group->next->pipefd[1], STDOUT_FILENO);
		close(group->next->pipefd[1]);
	}
}

void	ft_pipe(t_group *group)
{
	if (group->next && group->next->operator == PIPE
		&& pipe(group->next->pipefd) == -1)
	{
		error_msg("pipe failed");
		return ;
	}
	group->pid = fork();
	if (group->pid == -1)
	{
		error_msg("fork failed");
		return ;
	}
	if (group->pid == 0)
	{
		dup_and_close(group);
		if (group->operator == OPEN_SUBSHELL)
			open_subshell(group);
		else if (group->cmd && group->cmd[0])
			simple_command(group);
		cleanup_and_exit(group, *group->exitcode);
	}
	if (group->previous && group->previous->operator == PIPE)
		close(group->previous->pipefd[0]);
	if (group->next && group->next->operator == PIPE)
		close(group->next->pipefd[1]);
}


void	pipeline(t_group *group)
{
	t_group	*current;
	
	current = group;
	while (current && current->next && current->next->operator == PIPE)
	{
		ft_pipe(current);
		current = current->next->next;
	}
	ft_pipe(current);
	current = group;
	while (current && current->next && current->next->operator == PIPE)
	{
		ft_waitpid(current);
		current = current->next->next;
	}
	ft_waitpid(current);
	executor(current->next);
}