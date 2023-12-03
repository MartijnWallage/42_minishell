/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/03 23:56:14 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_group *group)
{
	int	exitcode;

	exitcode = 0;
	if (group->cmd[1])
		exitcode = ft_atoi(group->cmd[1]);
	cleanup_and_exit(group, exitcode);
}

void	builtin_env(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
}

void	builtin_pwd(char **env)
{
	char	*pwd;

	pwd = mini_getenv(env, "PWD");
	printf("%s\n", pwd);
}

void	builtin_echo(t_group *group)
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
}

void	builtin(t_group	*group)
{
	if (ft_strcmp(group->cmd[0], "cd") == 0)
		builtin_cd(group);
	else if (ft_strcmp(group->cmd[0], "exit") == 0)
		builtin_exit(group);
	else if (ft_strcmp(group->cmd[0], "env") == 0)
		builtin_env(group->env);
	else if (ft_strcmp(group->cmd[0], "pwd") == 0)
		builtin_pwd(group->env);
	else if (ft_strcmp(group->cmd[0], "echo") == 0)
		builtin_echo(group);
	else if (ft_strcmp(group->cmd[0], "export") == 0)
		builtin_export(group);
	else if (ft_strcmp(group->cmd[0], "unset") == 0)
		builtin_unset(group);
	if (group->operator == PIPE)
		cleanup_and_exit(group, 0);
}

bool	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	return (false);
}
