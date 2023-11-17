/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/11/17 18:05:25 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Features (implemented)
	- it changes the directory
	- it changes PWD
	- it changes OLDPWD
	- handles relative path

	To do:
	- error handling (not sure if necessary)
	- check for memory leaks
*/

char	*update_env2(char **env, char *new_path, char *key)
{
	int		i;
	int		len;
	char	*old_path;
	char	*full_path;

	len = ft_strlen(key);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], key, len) == 0)
		{
			old_path = get_value(env[i]);
			free(env[i]);
			if (new_path[0] != '/' && new_path[0] != '~')
			{
				full_path = ft_strdup(ft_strjoin(old_path, (ft_strjoin("/", new_path))));
				free(new_path);
				env[i] = ft_strdup(ft_strjoin(key, full_path));
				free(full_path);
			}
			else
				env[i] = ft_strdup(ft_strjoin(key, new_path));
			return (old_path);
		}
	}
	return (NULL);
}

void	update_old_pwd(char **env, char *line, char *key)
{
	int		i;

	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
		{
			free(env[i]);
			env[i] = (ft_strjoin(key, line));
			free(line);
		}
	}
}

void	builtin_cd(t_group *group)
{
	char	*old_pwd;

	if (chdir(group->cmd[1]) == 0)
	{
		old_pwd = update_env2(group->env, group->cmd[1], "PWD=");
		update_old_pwd(group->env, old_pwd, "OLDPWD=");
		free(old_pwd);
	}
}