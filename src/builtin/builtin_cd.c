/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/11/09 12:37:56 by jmuller          ###   ########.fr       */
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

char	*update_env2(char **env, char *line, char *key)
{
	int		i;
	char	*str;
	char	*full_path;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
		{
			str = ft_strdup(get_value(env[i]));
			free(env[i]);
			if (str[0] != line[0])
			{	
				full_path = ft_strdup(ft_strjoin(str, (ft_strjoin("/", line))));
				//free(line);
				env[i] = ft_strdup(ft_strjoin(key, full_path));
			}
			else
				env[i] = ft_strdup(ft_strjoin(key, line));
			break;
		}
		i++;
	}
	return (str);
}

void	update_env3(char **env, char *line, char *key)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
		{
			free(env[i]);
			env[i] = (ft_strjoin(key, line));
			free(line);
		}
		i++;
	}
	return ;
}

void	builtin_cd(t_group *group)
{
	char	*oldpwd;

	if (chdir(group->cmd[1]) == 0)
	{
		oldpwd = update_env2(group->env, group->cmd[1], "PWD=");
		update_env3(group->env, oldpwd, "OLDPWD=");
	}
}