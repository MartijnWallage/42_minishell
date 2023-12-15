/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:58 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/15 15:22:48 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	exec(t_group *group)
{
	char	*path;

	path = get_path(group->cmd[0], *group->env_ptr);
	protect_malloc(group, path);
	rl_clear_history();
	if (execve(path, group->cmd, *group->env_ptr) == -1)
	{
		if (ft_strncmp(path, group->cmd[0], ft_strlen(group->cmd[0])))
			free(path);
		ft_putstr_fd("philoshell: ", 2);
		ft_putstr_fd(group->cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		cleanup_and_exit(group, errno);
	}
}

void	ft_execute(t_group *group)
{
	if (group->pid == 0)
		exec(group);
	else
	{
		group->pid = fork();
		if (group->pid == 0)
			exec(group);
		ft_waitpid(group);
	}
}

void	*simple_command(t_group *group)
{
	if (!group->cmd || !group->cmd[0])
		return (NULL);
	expand_heredoc(group);
	if (!redirect(group))
		return (restore_redirection(group));
	expander(group);
	if (group->cmd[0] == NULL)
		return (restore_redirection(group));
	if (builtin(group))
	{
		if (group->pid == 0)
			cleanup_and_exit(group, 0);
	}
	else
		ft_execute(group);
	return (restore_redirection(group));
}

void	open_subshell(t_group *subshell, t_group *supershell)
{
	supershell->pid = fork();
	if (supershell->pid == -1)
	{
		error_msg("could not create subshell");
		return ;
	}
	if (supershell->pid == 0)
	{
		cleanup(supershell);
		executor(subshell);
		cleanup_and_exit(subshell, *subshell->exitcode); // probably redundant
	}
	ft_waitpid(supershell);
}

/**
 * @brief Recursively executes control groups by breaking them down 
 * into executable commands.
 * 
 * his function processes a list of control groups, 
 * each potentially representing a complex command structure.
 * We distinguish between complex command structure, complete commands 
 * and simple commands
 * 
 * It handles different types of control groups as follows:
 * a) Executes simple commands directly.
 * b) If cp is part of a pipeline, it activates the pl execution function.
 * c) Manages subshells, executing the enclosed commands as either sc or pls.
 * d) Evaluates logical operators (AND, OR) to determine the flow of execution.
 * 
 * The function works like this:
 * 1. Sets the signal handler to NON_INTERACTIVE MODE
 * 2. If pipe in the next group, the pipeline function is activated
 * 3. If open paranthesis, then the opensubshell function is activated
 * 4. The function checks for logical operators
 * 5. If no group operator is found, then simple command is executed
 * 6. The function goes through the list until it finds the end of a set of
 * control groups that form a complete command by checking pipes.
 * @param group A pointer to the head of the list of cgs to be executed.
 * @return Returns after the each control group was executed.
 */

void	executor(t_group *group)
{
	if (group == NULL)
		return ;
	handle_signals(NON_INTERACTIVE);
	if (group->operator == CLOSE_SUBSHELL)
		cleanup_and_exit(group, *group->exitcode);
	if (group->next && group->next->operator == PIPE)
		pipeline(group);
	else if (group->operator == OPEN_SUBSHELL)
		open_subshell(group->subshell, group);
	else if (group->operator == AND && *group->exitcode != 0)
		group = group->next;
	else if (group->operator == OR && *group->exitcode == 0)
		group = group->next;
	else if (group->operator == NONE)
		simple_command(group);
	group = next_complete_command(group);
	executor(group);
}
