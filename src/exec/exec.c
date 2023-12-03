/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:44:55 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 23:58:28 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_paths(char *cmd, char **paths)
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

static char	*get_path(char *cmd, char **env)
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
