/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 09:44:07 by jmuller           #+#    #+#             */
/*   Updated: 2024/01/07 12:52:34 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static void	update_oldpwd(t_group *group)
{
	int		i;
	char	*oldpwd;

	if (mini_getenv(*group->env_ptr, "PWD") == NULL)
	{
		unset_var(*group->env_ptr, "OLDPWD");
		return ;
	}
	oldpwd = ft_strjoin("OLDPWD=", mini_getenv(*group->env_ptr, "PWD"));
	protect_malloc(group, oldpwd);
	i = 0;
	while ((*group->env_ptr)[i]
		&& ft_strncmp((*group->env_ptr)[i], "OLDPWD=", 7))
		i++;
	if ((*group->env_ptr)[i])
	{
		free((*group->env_ptr)[i]);
		(*group->env_ptr)[i] = oldpwd;
	}
	else
	{
		append_var(group, oldpwd);
		free(oldpwd);
	}
}

static void	update_pwd(t_group *group)
{
	int		i;
	char	buffer[1024];
	char	*new_pwd;

	new_pwd = ft_strjoin("PWD=", getcwd(buffer, 1024));
	protect_malloc(group, new_pwd);
	if (mini_getenv(*group->env_ptr, "PWD") == NULL)
	{
		append_var(group, new_pwd);
		free(new_pwd);
	}
	else
	{
		i = 0;
		while ((*group->env_ptr)[i]
			&& ft_strncmp((*group->env_ptr)[i], "PWD=", 4))
			i++;
		free((*group->env_ptr)[i]);
		(*group->env_ptr)[i] = new_pwd;
	}
}

static int	goto_home(t_group *group)
{
	char	*home;
	char	*temp;

	home = mini_getenv(*group->env_ptr, "HOME");
	if (!home)
	{
		*group->exitcode = error_msg("cd: HOME not set");
		return (0);
	}
	if (group->cmd[1])
	{
		temp = group->cmd[1];
		group->cmd[1] = ft_strjoin(home, group->cmd[1] + 1);
		free(temp);
		protect_malloc(group, group->cmd[1]);
	}
	else
		insert_word(group, home, 1);
	return (1);
}

int	builtin_cd(t_group *group)
{
	char	*msg;

	if (group->cmd[1] && group->cmd[2])
	{
		*group->exitcode = error_msg("cd: too many arguments");
		return (1);
	}
	if ((group->cmd[1] == NULL || group->cmd[1][0] == '~') && !goto_home(group))
		return (1);
	if (chdir(group->cmd[1]) == 0)
	{
		update_oldpwd(group);
		update_pwd(group);
		*group->exitcode = 0;
	}
	else
	{
		msg = ft_strjoin("cd: ", group->cmd[1]);
		protect_malloc(group, msg);
		*group->exitcode = error_msg(msg);
		free(msg);
	}
	return (1);
}
