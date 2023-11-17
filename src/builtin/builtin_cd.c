/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/11/17 23:17:37 by mwallage         ###   ########.fr       */
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
	if (old_path[len - 1] == '/')
		len--;
	while (len-- && old_path[len] != '/')
		;
	if (len == 0)
		len++;
	higher_dir = ft_substr(old_path, 0, len);
	return (higher_dir);
}

/* char	*get_previous_dir(char **env)
{
	// find OLDPWD
	// that's the new PWD
} */

char	*update_pwd(char **env, char *new_path)
{
	char	*old_path;
	char	*full_path;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PWD=", 4))
		i++;
	old_path = get_value(env[i]);
	if (new_path[0] == '/' || new_path[0] == '~')
	{
		env[i] = ft_strjoin("PWD=", new_path);
		return (old_path);
	}
	if (ft_strncmp(new_path, "..", 2) == 0)
		full_path = get_higher_dir(old_path);
	else
		full_path = ft_strjoin(old_path, (ft_strjoin("/", new_path)));
	env[i] = ft_strjoin("PWD=", full_path);		
	return (old_path);
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
 		old_path =  update_pwd(group->env, group->cmd[1]);
		update_oldpwd(group->env, old_path);
		free(old_path);
	}
}