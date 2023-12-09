/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:14:58 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 21:52:03 by mwallage         ###   ########.fr       */
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

void	simple_command(t_group *group)
{
	if (!group->cmd || !group->cmd[0])
		return ;
	ft_putstr_fd("Simple command: ", group->original_stdout);
	ft_putendl_fd(group->cmd[0], group->original_stdout);
	if (!redirect(group))
	{
		if (group->pid == 0)
			cleanup_and_exit(group, 1);
		else
			return ;
	}
	expander(group);
	if (builtin(group))
	{
		if (group->pid == 0)
			cleanup_and_exit(group, 0);
	}
	else if (group->pid == 0)
		exec(group);
	else
	{
		group->pid = fork();
		if (group->pid == 0)
			exec(group);
		ft_waitpid(group);
	}
	restore_redirection(group);
}

void	open_subshell(t_group *group)
{
	t_group	*list;

	list = parser(&group->cmd[1], group->env_ptr, group->exitcode);
	expander(list);
	group->pid = fork();
	if (group->pid == -1)
	{
		error_msg("could not create subshell");
		return ;
	}
	if (group->pid == 0)
		executor(list);
	else
		ft_waitpid(group);
}

void	executor(t_group *group)
{
	if (group == NULL)
		return ;
	if (group->operator == CLOSE_SUBSHELL)
		cleanup_and_exit(group, *group->exitcode);
 	if (group->next && group->next->operator == PIPE)
		pipeline(group);
	else if (group->operator == OPEN_SUBSHELL)
	{
		open_subshell(group);
		executor(group->next);
	}
	else if (group->operator == AND && *group->exitcode == 0)
		executor(group->next);
	else if (group->operator == AND)
		executor(skip_next_complete_command(group->next));
	else if (group->operator == OR && *group->exitcode != 0)
		executor(group->next);
	else if (group->operator == OR)
		executor(skip_next_complete_command(group->next));
	else if (group->operator == NONE)
	{
		simple_command(group);
		executor(group->next);
	}
}
