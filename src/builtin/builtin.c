/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/09 09:37:41 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_exit(t_group *group)
{
	cleanup(group);
	exit(0);
}

void	builtin_env(t_group *group)
{
	int	i;

	i = 0;
	while (group->env[i])
	{
		printf("%s\n", group->env[i]);
		i++;
	}
}

void	builtin_pwd(t_group *group)
{
	int	i;

	i = 0;
	while (group->env[i])
	{
		if (ft_strncmp(group->env[i], "PWD", 3) == 0)
		{
			printf("%s\n", &(group->env[i][4]));
			break ;
		}
		i++;
	}
}

void	builtin_echo(t_group *group)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (group->cmd[1] && ft_strcmp(group->cmd[1], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	while (group->cmd[i])
	{
		printf("%s", group->cmd[i]);
		i++;
		if (group->cmd[i])
			printf(" ");
	}
	if (flag == 0)
		printf("\n"); 
}

void	builtin(t_group	*group)
{
	if (ft_strcmp(group->cmd[0], "exit") == 0)
		builtin_exit(group);
	if (ft_strcmp(group->cmd[0], "env") == 0)
		builtin_env(group);
	if (ft_strcmp(group->cmd[0], "pwd") == 0)
		builtin_pwd(group);
	if (ft_strcmp(group->cmd[0], "echo") == 0)
		builtin_echo(group);
	if (ft_strcmp(group->cmd[0], "export") == 0)
		builtin_export(group);
	if (ft_strcmp(group->cmd[0], "unset") == 0)
		builtin_unset(group);
	if (group->operator == PIPE)
	{
		cleanup(group);
		exit(0);
	}
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
