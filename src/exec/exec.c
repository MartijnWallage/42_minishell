/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:44:55 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/14 15:28:06 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_paths(char *cmd, char **paths)
{
	int		i;
	char	*whole_cmd;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		whole_cmd = ft_strjoin(paths[i], "/");
		temp = whole_cmd;
		whole_cmd = ft_strjoin(whole_cmd, cmd);
		if (temp != whole_cmd)
			free(temp);
		if (access(whole_cmd, F_OK | X_OK) == 0)
			return (whole_cmd);
		free(whole_cmd);
		i++;
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
		if (ft_strncmp(line[0], "PATH", 4) == 0)
			break ;
		free_tab(line);
	}
	paths = ft_split(line[1], ':');
	free_tab(line);
	whole_cmd = try_paths(cmd, paths);
	free_tab(paths);
	return (whole_cmd);
}

void	exec(char **cmd, char **env)
{
	char	*path;

	path = get_path(cmd[0], env);
	if (execve(path, cmd, env) == -1)
	{
		if (ft_strcmp(path, cmd[0]))
			free(path);
		ft_putstr_fd("philoshell: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_tab(cmd);
		exit(errno);
	}
}
