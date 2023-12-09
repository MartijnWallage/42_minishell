/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 21:15:09 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	builtin_exit(t_group *group)
{
	int	exitcode;

	exitcode = 0;
	if (group->cmd[1])
		exitcode = ft_atoi(group->cmd[1]);
	printf("exit\n");
	cleanup_and_exit(group, exitcode);
	return (1);
}

int	builtin_env(t_group *group)
{
	int		i;
	char	**env;

	env = *group->env_ptr;
	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	*group->exitcode = 0;
	return (1);
}

int	builtin_pwd(t_group *group)
{
	char	*pwd;

	pwd = mini_getenv(*group->env_ptr, "PWD");
	printf("%s\n", pwd);
	*group->exitcode = 0;
	return (1);
}

int	builtin_echo(t_group *group)
{
	int	i;

	i = 1;
	if (group->cmd[i] && !ft_strcmp(group->cmd[i], "-n"))
		i++;
	while (group->cmd[i])
	{
		printf("%s", group->cmd[i]);
		i++;
		if (group->cmd[i])
			printf(" ");
	}
	if (!group->cmd[1] || ft_strncmp(group->cmd[1], "-n", 3))
		printf("\n");
	*group->exitcode = 0;
	return (1);
}

int	builtin(t_group	*group)
{
	if (ft_strncmp(group->cmd[0], "cd", 3) == 0)
		return (builtin_cd(group));
	if (ft_strncmp(group->cmd[0], "exit", 5) == 0)
		return (builtin_exit(group));
	if (ft_strncmp(group->cmd[0], "env", 4) == 0)
		return (builtin_env(group));
	if (ft_strncmp(group->cmd[0], "pwd", 4) == 0)
		return (builtin_pwd(group));
	if (ft_strncmp(group->cmd[0], "echo", 5) == 0)
		return (builtin_echo(group));
	if (ft_strncmp(group->cmd[0], "export", 7) == 0)
		return (builtin_export(group));
	if (ft_strncmp(group->cmd[0], "unset", 6) == 0)
		return (builtin_unset(group));
	return (0);
}
