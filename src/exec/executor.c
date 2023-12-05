/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:58 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 10:55:34 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	exec(t_group *group)
{
	char	*path;

	path = get_path(group->cmd[0], group->env);
	protect_malloc(group, path);
	rl_clear_history();
	if (execve(path, group->cmd, group->env) == -1)
	{
		if (ft_strncmp(path, group->cmd[0], ft_strlen(group->cmd[0])))
			free(path);
		ft_putstr_fd("philoshell: command not found: ", 2);
		ft_putendl_fd(group->cmd[0], 2);
		cleanup_and_exit(group, errno);
	}
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
	if (builtin(group))
	{
		if (group->operator == PIPE)
			cleanup_and_exit(group, 0);
		else
		{
			restore_redirection(group);
			return ;
		}
	}
	if (group->operator == PIPE)
		exec(group);
	group->pid = fork();
	if (group->pid == 0)
		exec(group);
	waitpid(group->pid, &status, 0);
	if (WIFEXITED(status))
		group->exitcode = WEXITSTATUS(status);
	else
		error_msg("program quit unexpectedly");
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
