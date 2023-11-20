

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
#include "minishell.h"

/*
 Syntax:
 - export var_name=value var_name2=value;

 Features:
 - allows for multiple var assignments in one go
 - checks for validity of key/value-pairs
 - updates key/value pairs
 - naked export command, gives "declare -x $env[i]"

To do:
- remove '""'

Questions:
- the variables are written to the env, such that even after
  minishell is exited, they are still there. Is this a problem?
*/

int	needle_check(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c && str[i + 1] != '\0')
			return (1);
		i++;
	}
	return (0);
}

/*
	needs proper exit code:
	Example: export var1= 1
	bash: export: `1': not a valid identifier
*/
int	key_valuecheck(char *str)
{
	char **kv_pair;
	int	i;

	i = 1;
	if (needle_check(str, '=') == 0)
		return (1);
	kv_pair = ft_split(str, '=');
	if ((ft_isdigit(kv_pair[0][0])) || !((ft_isalpha(kv_pair[0][0]) || \
		kv_pair[0][0] == '_')))
	{	
		free_tab(kv_pair);
		return (1);
	}

	while (kv_pair[0][i])
	{
		if (!(ft_isalnum(kv_pair[0][i]) || kv_pair[0][i] == '_'))
		{
			free_tab(kv_pair);
			return (1);
		}
		i++;
	}
	i = 0;
	while (kv_pair[1][i])
	{
		if (!(ft_isascii(kv_pair[1][i])) || kv_pair[1][0] == ' ')
		{
			free_tab(kv_pair);
			return (1);
		}
		i++;
	}
	free_tab(kv_pair);
	return (0);
}

void	update_env1(char **env, char *line)
{
	int		i;
	char	*key;

	i = 0;
	key = get_key(line);
	while (env[i])
	{
		if (ft_strncmp(env[i], key, ft_strlen(key)) == 0)
		{
			free(env[i]);
			env[i] = ft_strdup(line);
		}
		i++;
	}
	free(key);
	return ;
}

void	append_env(t_group *group, char *line)
{
	int		i;
	int		row;
	char	**env2;

	i = 0;
	row = tab_len(group->env);
	env2 = malloc(sizeof(char *) * (row + 2));
	// protect malloc
	while (i < row)
	{
		env2[i] = ft_strdup(group->env[i]);
		i++;
	}
	env2[i] = ft_strdup(line);
	env2[i + 1] = NULL;
	free_tab(group->env);
	group->env = env2;
}

void	naked_export(t_group *group)
{
	int	i;

	i = 0;
	while (group->env[i])
	{
		printf("declare -x ");
		printf("%s\n", group->env[i]);
		i++;
	}
}

void	builtin_export(t_group *group)
{
	int i;

	if (!group->cmd[1])
	{
		naked_export(group);
		return ;
	}
	i = -1;
	while (group->cmd[++i])
	{
		if (key_valuecheck(group->cmd[i]))
			break ;
		if (key_compare(group->env, group->cmd[i]))
			update_env1(group->env, group->cmd[i]);
		else
			append_env(group, group->cmd[i]); //need explanation
	}
}
