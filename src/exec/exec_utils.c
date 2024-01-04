/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:44:55 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/13 16:08:12 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*try_paths(t_group *group, char *cmd, char **paths)
{
	int		i;
	char	*whole_cmd;
	char	*temp;

	i = -1;
	while (paths[++i])
	{
		whole_cmd = ft_strjoin(paths[i], "/");
		if (!whole_cmd)
			cleanup_and_exit(group, MALLOC_CODE);
		temp = whole_cmd;
		whole_cmd = ft_strjoin(whole_cmd, cmd);
		free(temp);
		if (!whole_cmd)
			cleanup_and_exit(group, MALLOC_CODE);
		if (access(whole_cmd, F_OK | X_OK) == 0)
			return (whole_cmd);
		free(whole_cmd);
	}
	return (NULL);
}

char	*get_path(t_group *group, char *cmd, char **env)
{
	char	*line;
	char	**paths;
	char	*whole_cmd;

	line = mini_getenv(env, "PATH");
	if (line == NULL)
		return (NULL);
	paths = ft_split(line, ':');
	if (!paths)
		cleanup_and_exit(group, MALLOC_CODE);
	whole_cmd = try_paths(group, cmd, paths);
	free_tab(paths);
	return (whole_cmd);
}

int	ft_dup2(t_group *group, int fd1, int fd2)
{
	if (fd2 == STDIN_FILENO && group->original_stdin == STDIN_FILENO)
		group->original_stdin = dup(STDIN_FILENO);
	else if (fd2 == STDOUT_FILENO && group->original_stdout == STDOUT_FILENO)
		group->original_stdout = dup(STDOUT_FILENO);
	return (dup2(fd1, fd2));
}

void	ft_waitpid(t_group *group)
{
	int	status;

	waitpid(group->pid, &status, 0);
	if (WIFEXITED(status))
		*group->exitcode = WEXITSTATUS(status);
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
