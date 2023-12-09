/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/12/05 11:00:29 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	update_oldpwd(t_group *group)
{
	int		i;
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", mini_getenv(*group->mini_env, "PWD"));
	protect_malloc(group, oldpwd);
	i = 0;
	while (*group->mini_env[i] && ft_strncmp(*group->mini_env[i], "OLDPWD=", 7))
		i++ ;
	if (*group->mini_env[i])
	{
		free(*group->mini_env[i]);
		*group->mini_env[i] = oldpwd;
	}
	else
		append_var(group, oldpwd);
}

static void	update_pwd(t_group *group)
{
	int		i;
	char	buffer[1024];
		
	i = 0;
	while (*group->mini_env[i] && ft_strncmp(*group->mini_env[i], "PWD=", 4))
		i++;
	free(*group->mini_env[i]);
	*group->mini_env[i] = ft_strjoin("PWD=", getcwd(buffer, 1024));
	protect_malloc(group, *group->mini_env[i]);
}

static void	goto_home(t_group *group)
{
	char	*home;
	char	*temp;

	home = mini_getenv(*group->mini_env, "HOME");
	if (home)
	{
		temp = group->cmd[1];
		group->cmd[1] = ft_strjoin(home, group->cmd[1] + 1);
		free(temp);
		protect_malloc(group, group->cmd[1]);
	}
}

static void	goto_previous_dir(t_group *group)
{
	char	*old_path;

	old_path = mini_getenv(*group->mini_env, "OLDPWD");
	if (old_path)
	{
		free(group->cmd[1]);	
		group->cmd[1] = ft_strdup(old_path);
		protect_malloc(group, group->cmd[1]);
	}
}

int	builtin_cd(t_group *group)
{
	if (!group->cmd[1])
		return (1);
	if (group->cmd[1][0] == '~')
		goto_home(group);
	else if (ft_strcmp(group->cmd[1], "-") == 0)
		goto_previous_dir(group);
	if (chdir(group->cmd[1]) == 0)
	{
		update_oldpwd(group);
		update_pwd(group);
	}
	*group->exitcode = 0;
	return (1);
}