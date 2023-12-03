/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2023/12/03 19:34:24 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_oldpwd(t_group *group)
{
	int		i;
	char	*oldpwd;

	oldpwd = ft_strjoin("OLDPWD=", mini_getenv(group->env, "PWD"));
	protect_malloc(group, oldpwd);
	i = 0;
	while (group->env[i] && ft_strncmp(group->env[i], "OLDPWD=", 7))
		i++ ;
	if (group->env[i])
	{
		free(group->env[i]);
		group->env[i] = oldpwd;
	}
	else
		append_var(group, oldpwd);
}

static void	update_pwd(t_group *group)
{
	int		i;
	char	buffer[1024];
		
	i = 0;
	while (group->env[i] && ft_strncmp(group->env[i], "PWD=", 4))
		i++;
	free(group->env[i]);
	group->env[i] = ft_strjoin("PWD=", getcwd(buffer, 1024));
	protect_malloc(group, group->env[i]);
}

static void	goto_home(t_group *group)
{
	char	*home;
	char	*temp;

	home = mini_getenv(group->env, "HOME");
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

	old_path = mini_getenv(group->env, "OLDPWD");
	if (old_path)
	{
		free(group->cmd[1]);	
		group->cmd[1] = ft_strdup(old_path);
		protect_malloc(group, group->cmd[1]);
	}
}

void	builtin_cd(t_group *group)
{
	if (!group->cmd[1])
		return ;
	if (group->cmd[1][0] == '~')
		goto_home(group);
	else if (ft_strcmp(group->cmd[1], "-") == 0)
		goto_previous_dir(group);
	if (chdir(group->cmd[1]) == 0)
	{
		update_oldpwd(group);
		update_pwd(group);
	}
}