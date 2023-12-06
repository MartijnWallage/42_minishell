/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:58 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 14:14:37 by mwallage         ###   ########.fr       */
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
	}
	else if (builtin(group))
	{
		if (group->operator == PIPE)
			cleanup_and_exit(group, 0);
	}
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

void	executor(t_group *group)
{
	if (group->operator == 0)
		simple_command(group);
	else if (group->operator == PIPE)
		pipeline(group);
}
