/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/11/17 18:53:04 by mwallage         ###   ########.fr       */
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

char	*get_higher_dir(char *old_path)
{
	int		len;
	char	*higher_dir;

	len = ft_strlen(old_path);
	while (old_path[len] != '/')
		len--;
	len--;
	higher_dir = malloc(len + 1);
	higher_dir[len] = 0;
	ft_strlcpy(higher_dir, old_path, len);
	return (higher_dir);
}

/* char	*get_previous_dir(char **env)
{
	// find OLDPWD
	// that's the new PWD
} */

char	*update_pwd(char **env, char *new_path)
{
	int		i;
	char	*old_path;
	char	*full_path;

	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "PWD=", 4) == 0)
		{
			old_path = get_value(env[i]);
			free(env[i]);
			if (new_path[0] != '/' && new_path[0] != '~')
			{
				if (ft_strcmp(new_path, "..") == 0)
					full_path = get_higher_dir(old_path);
/* 				else if (ft_strcmp(new_path, "-") == 0)
					full_path = get_previous_dir(env); */
				else
					full_path = ft_strdup(ft_strjoin(old_path, (ft_strjoin("/", new_path))));
				env[i] = ft_strdup(ft_strjoin("PWD=", full_path));
				free(full_path);
			}
			else
				env[i] = ft_strdup(ft_strjoin("PWD=", new_path));
			return (old_path);
		}
	}
	return (NULL);
}

void	update_oldpwd(char **env, char *old_path)
{
	int		i;

	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			free(env[i]);
			env[i] = ft_strjoin("OLDPWD=", old_path);
		}
	}
}

void	builtin_cd(t_group *group)
{
	char	*old_path;

	if (chdir(group->cmd[1]) == 0)
	{
		old_path = update_pwd(group->env, group->cmd[1]);
		update_oldpwd(group->env, old_path);
		free(old_path);
	}
}