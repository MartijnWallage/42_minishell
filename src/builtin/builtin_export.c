

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/07 10:44:19 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/*
 *	Syntax:
 *	- export var_name=value var_name2=value;
 *
 *	Features:
 *	- allows for multiple var assignments in one go
 *	- checks for validity of key/value-pairs
 *	- updates key/value pairs
 *	- naked export command, gives "declare -x $env[i]"

 *	To do:
 *	- proper exit codes
 */

static int	is_valid_arg(char *str)
{
	char	*equals_sign;

	equals_sign = ft_strchr(str, '=');
	if (!equals_sign || (!isalpha(*str) && *str != '_'))
		return (0);
	while (++str != equals_sign)
		if (!(ft_isalnum(*str) && *str != '_'))
			return (0);
	while (*++str)
		if (!ft_isascii(*str))
			return (0);
	return (1);
}

void	append_var(t_group *group, char *var)
{
	int		i;
	int		tablen;
	char	**new_env;
	char	**old_env;

	old_env = *group->env_ptr;
	tablen = tab_len(old_env);
	new_env = malloc(sizeof(char *) * (tablen + 2));
	protect_malloc(group, new_env);
	i = -1;
	while (++i < tablen)
	{
		new_env[i] = ft_strdup((*group->env_ptr)[i]);
		protect_malloc(group, new_env[i]);
	}
	new_env[i] = ft_strdup(var);
	protect_malloc(group, new_env[i]);
	new_env[i + 1] = NULL;
	free_tab(old_env);
	*group->env_ptr = new_env;
}


static void	update_env(t_group *group, char *var)
{
	int		i;
	int		j;
	char	**env;

	env = *group->env_ptr;
	i = -1;
	while (env[++i])
	{
		j = 0;
		while (var[j] != '=' && env[i][j] == var[j])
			++j;
		if (var[j] == '=')
		{
			free(env[i]);
			env[i] = ft_strdup(var);
			protect_malloc(group, env[i]);
			return ;
		}
	}
	append_var(group, var);
}

static void	export_without_arg(char ***env_ptr)
{
	char	**env;

	env = *env_ptr;
	while (*env)
	{
		printf("declare -x ");
		printf("%s\n", *env);
		env++;
	}
}

int	builtin_export(t_group *group)
{
	int	i;

	i = 1;
	if (group->cmd[i] == NULL)
		export_without_arg(group->env_ptr);
	else while (group->cmd[i])
	{
		if (is_valid_arg(group->cmd[i]))
			update_env(group, group->cmd[i]);
		i++;
	}
	*group->exitcode = 0;
	return (1);
}
