/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:44:55 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 10:33:42 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*try_paths(char *cmd, char **paths)
{
	int		i;
	char	*whole_cmd;
	char	*temp;

	i = -1;
	while (paths[++i])
	{
		whole_cmd = ft_strjoin(paths[i], "/");
		if (!whole_cmd)
			return (NULL);
		temp = whole_cmd;
		whole_cmd = ft_strjoin(whole_cmd, cmd);
		free(temp);
		if (!whole_cmd)
			return (NULL);
		if (access(whole_cmd, F_OK | X_OK) == 0)
			return (whole_cmd);
		free(whole_cmd);
	}
	return (cmd);
}

char	*get_path(char *cmd, char **env)
{
	char	**line;
	char	**paths;
	char	*whole_cmd;
	int		i;

	if (!env)
		return (cmd);
	i = 0;
	while (1)
	{
		if (!env[i])
			return (cmd);
		line = ft_split(env[i++], '=');
		if (!line)
			return (NULL);
		if (ft_strncmp(line[0], "PATH", 4) == 0)
			break ;
		free_tab(line);
	}
	paths = ft_split(line[1], ':');
	free_tab(line);
	if (!paths)
		return (NULL);
	whole_cmd = try_paths(cmd, paths);
	free_tab(paths);
	return (whole_cmd);
}

int	ft_dup2(t_group *group, int fd1, int fd2)
{
	if (fd2 == STDIN_FILENO && group->original_stdin == STDIN_FILENO)
		group->original_stdin = dup(STDIN_FILENO);
	if (fd2 == STDOUT_FILENO && group->original_stdout == STDOUT_FILENO)
		group->original_stdout = dup(STDOUT_FILENO);
	return (dup2(fd1, fd2));
}

void	ft_waitpid(t_group *group)
{
	int	status;

	waitpid(group->pid, &status, 0);
	if (WIFEXITED(status))
		*group->exitcode = WEXITSTATUS(status);
	else if (WEXITSTATUS(status))
	{
		if (group->cmd[0])
			error_msg(group->cmd[0]);
		else
			error_msg("program quit unexpectedly");
	}
}

/// @brief skip next simple command or the next pipeline, if there is one
/// @param group current group (the first group after && or ||)
/// @return the next group that the executor should evaluate
t_group	*next_complete_command(t_group *group)
{
	if (!group)
		return (NULL);
	while (group->next && group->next->operator == PIPE)
		group = group->next->next;
	group = group->next;
	return (group);
}
