/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:58 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/03 14:01:03 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	simple_command(t_group *group)
{
	pid_t	pid;
	
	if (is_builtin(group->cmd[0]))
		builtin(group);
	else if (group->operator == PIPE)
		exec(group->cmd, group->env);
	else
	{
	  	pid = fork();
		if (pid == 0)
			exec(group->cmd, group->env);
		waitpid(pid, NULL, 0);
	}
}

static void	child(t_group *group)
{
	pid_t	pid;

	if (group->next && pipe(group->pipefd) == -1)
		handle_error("pipe error", 1);
	pid = fork();
	if (pid == -1)
		handle_error("pid error", 1);
	if (pid == 0)
	{
		if (group->previous)
		{
			dup2(group->previous->pipefd[0], STDIN_FILENO);
			close(group->previous->pipefd[0]);
		}
		if (group->next)
		{
			close(group->pipefd[0]);
			dup2(group->pipefd[1], STDOUT_FILENO);
			close(group->pipefd[1]);
		}
		simple_command(group);
	}
	if (group->next)
		close(group->pipefd[1]);
	else
		waitpid(pid, NULL, 0);
}

void	executor(t_group *group)
{
	t_group	*current;

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
}
