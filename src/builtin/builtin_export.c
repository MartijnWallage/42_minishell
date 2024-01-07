/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:10:00 by mwallage          #+#    #+#             */
/*   Updated: 2024/01/07 13:12:40 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static int	is_valid_arg(char *str)
{
	char	*equals_sign;

	equals_sign = ft_strchr(str, '=');
	if (!equals_sign || (!isalpha(*str) && *str != '_'))
		return (0);
	while (++str != equals_sign)
		if (!(ft_isalnum(*str) || *str == '_'))
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
		protect_and_free(group, new_env[i], new_env, NULL);
	}
	new_env[i] = ft_strdup(var);
	protect_and_free(group, new_env[i], new_env, NULL);
	new_env[i + 1] = NULL;
	free_tab(old_env);
	*group->env_ptr = new_env;
}

static int	update_env(t_group *group, char *var)
{
	int		i;
	int		j;
	char	**env;
	char	*new_envi;

	env = *group->env_ptr;
	i = -1;
	while (env[++i])
	{
		j = 0;
		while (var[j] != '=' && env[i][j] == var[j])
			++j;
		if (var[j] == '=')
		{
			new_envi = ft_strdup(var);
			protect_malloc(group, new_envi);
			free(env[i]);
			env[i] = new_envi;
			return (1);
		}
	}
	append_var(group, var);
	return (1);
}

static int	export_without_arg(char ***env_ptr)
{
	char	**env;

	env = *env_ptr;
	while (*env)
	{
		printf("declare -x ");
		printf("%s\n", *env);
		env++;
	}
	return (1);
}

int	builtin_export(t_group *group)
{
	int		i;
	char	*msg;
	char	*temp;

	i = 0;
	if (group->cmd[1] == NULL)
	{
		*group->exitcode = 0;
		return (export_without_arg(group->env_ptr));
	}
	while (group->cmd[++i])
	{
		if (is_valid_arg(group->cmd[i]))
			*group->exitcode = !update_env(group, group->cmd[i]);
		else
		{
			msg = ft_strjoin("export: '", group->cmd[i]);
			protect_malloc(group, msg);
			temp = msg;
			msg = ft_strjoin(msg, "' is not a valid identifier");
			free(temp);
			protect_malloc(group, msg);
			*group->exitcode = error_msg(msg);
			free(msg);
		}
	}
	return (1);
}
