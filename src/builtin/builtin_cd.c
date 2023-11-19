/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/11/19 19:07:51 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Features (implemented)
	- it changes the directory
	- it changes PWD
	- it changes OLDPWD
	- handles relative path
	- cd .., cd ../path, cd ~, cd /, cd -

	To do:
	- error handling (not sure if necessary)
	- check for memory leaks
*/

char	*get_higher_dir(char *old_path)
{
	int		len;
	char	*higher_dir;

	len = ft_strlen(old_path);
	while (len-- && old_path[len] != '/')
		;
	if (len == 0)
		len++;
	higher_dir = ft_substr(old_path, 0, len);
	return (higher_dir);
}

void	update_oldpwd(t_group *group, char *old_path)
{
	int		i;
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", old_path);
	i = 0;
	while (group->env[i] && ft_strncmp(group->env[i], "OLDPWD=", 7))
		i++ ;
	if (group->env[i])
	{
		free(group->env[i]);
		group->env[i] = oldpwd;
	}
	else
		append_env(group, oldpwd);
}

char	*update_pwd(char **env, char *new_path)
{
	char	*old_path;
	char	*full_path;
	int		i;
	int		len;
	
	len = ft_strlen(new_path);
	if (len > 1 && new_path[len - 1] == '/')
		new_path[len - 1] = 0;
	old_path = ft_strdup(mini_getenv(env, "PWD"));
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD=", 4))
		i++;
	if (new_path[0] == '/')
	{
		env[i] = ft_strjoin("PWD=", new_path);
		return (old_path);
	}
	if (ft_strncmp(new_path, "..", 2) == 0)
		full_path = get_higher_dir(old_path);
	else if (old_path[ft_strlen(old_path) - 1] == '/')
		full_path = ft_strjoin(old_path, new_path);
	else
		full_path = ft_strjoin(old_path, ft_strjoin("/", new_path));
	env[i] = ft_strjoin("PWD=", full_path);		
	return (old_path);
}


void	builtin_cd(t_group *group)
{
	char	*old_path;
	char	*home;
	char	*temp;

	if (group->cmd[1][0] == '~')
	{
		home = mini_getenv(group->env, "HOME");
		temp = group->cmd[1];
		group->cmd[1] = ft_strjoin(home, group->cmd[1] + 1);
		free(temp);
	}
	else if (ft_strcmp(group->cmd[1], "-") == 0)
	{
		old_path = mini_getenv(group->env, "OLDPWD");
		if (old_path)
		{
			free(group->cmd[1]);
			group->cmd[1] = ft_strdup(old_path);
		}
	}
	if (chdir(group->cmd[1]) == 0)
	{
		old_path = update_pwd(group->env, group->cmd[1]);
		update_oldpwd(group, old_path);
		free(old_path);
	}
}