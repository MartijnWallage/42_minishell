

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
  Syntax
  export var_name=value var_name2=value;
1.  Lets do a simple version, then make it more diff
--a. Get group->cmd[1];
--b. get its len
--c. duplicate group and expand to create env2
--d. write group->cmd[1]; in env2


*/

void	free_doublearray(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}


int	row_count(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	builtin_export(t_group *group)
{
	int	row;
	int	i;
	char **env2;
	
	i = 0;
	row = row_count(group->env);
	env2 = malloc(sizeof(char *) * (row + 2)); // bcz of 1 extra row
	// protect malloc
	while (i < row)
	{
		env2[i] = ft_strdup(group->env[i]);
		i++;
	}
	//free_doublearray(group->env);
	env2[i] = ft_strdup(group->cmd[1]);
	env2[i + 1] = NULL;
	group->env = env2;
	i = 0;
	while (group->env[i])
	{
		printf("%s\n", group->env[i]);
		i++;
	}

}
