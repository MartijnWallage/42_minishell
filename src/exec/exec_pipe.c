/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 15:20:23 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/19 16:50:07 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

/**
 * @brief redirect stdin to the read end of the previous pipe, 
 * and close that pipe, or
 * redirect stdout to the write end of the next pipe, and close that pipe.
 * @param group the current group in the pipeline
 */

void	dup_and_close(t_group *group)
{
	if (group->previous && group->previous->operator == PIPE)
	{
		if (ft_dup2(group, group->previous->pipefd[0], STDIN_FILENO) == -1)
			cleanup_and_exit(group, errno);
		close(group->previous->pipefd[0]);
	}
	if (group->next && group->next->operator == PIPE)
	{
		close(group->next->pipefd[0]);
		if (ft_dup2(group, group->next->pipefd[1], STDOUT_FILENO) == -1)
			cleanup_and_exit(group, errno);
		close(group->next->pipefd[1]);
	}
}

/**	
 * @brief	pipe, fork, and in the child process, run a simple command
 * 			or a subshell, which is treated as a simple command.
 * @param group	current group in the pipeline
 */

void	ft_pipe(t_group *group)
{
	if (group->next && group->next->operator == PIPE
		&& pipe(group->next->pipefd) == -1)
		cleanup_and_exit(group, errno);
	group->pid = fork();
	if (group->pid == -1)
		cleanup_and_exit(group, errno);
	if (group->pid == 0)
	{
		dup_and_close(group);
		if (group->operator == OPEN_SUBSHELL)
			open_subshell(group->subshell, group);
		else if (group->cmd && group->cmd[0])
			simple_command(group);
		cleanup_and_exit(group, *group->exitcode);
	}
	if (group->previous && group->previous->operator == PIPE)
		close(group->previous->pipefd[0]);
	if (group->next && group->next->operator == PIPE)
		close(group->next->pipefd[1]);
}

/**
 * @brief Executes a series of commands connected by pipes.
 *
 * This function manages the execution of command groups linked by pipes.
 * 1. 	Iterates through the list of control groups
 *		and creates pipes between commands.
 *   	If there is a pipeline it pipes the result
 *		from the first to the last simple command
 * 2. 	Executes each command in the pipeline, 
 *    	where each command execution is potentially in its own child process.
 * 3. 	Waits for all child processes to terminate, 
 *    	ensuring the complete execution of the pipeline.
 * @param group Pointer to the head of a list of control groups,
 *				representing the pipeline
 * @return Implicitly returns when all waitpid functions terminate
 */

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
}
