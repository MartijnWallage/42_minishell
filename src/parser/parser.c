/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 12:38:56 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/// @brief 			Initialize group
/// @param cmd 		list of tokens passed by tokenizer
/// @param env 		copy of environment
/// @param exitcode last known exitcode
/// @return 		the initalized group
static t_group	*init_group(char **cmd, char ***env_ptr, int *exitcode)
{
	t_group	*list;

	list = malloc(sizeof(t_group));
	protect_malloc_during_build(cmd, env_ptr, list);
	list->cmd = NULL;
	list->operator = NONE;
	list->previous = NULL;
	list->next = NULL;
	list->env_ptr = env_ptr;
	list->infile = STDIN_FILENO;
	list->outfile = STDOUT_FILENO;
	list->original_stdin = STDIN_FILENO;
	list->original_stdout = STDOUT_FILENO;
	list->exitcode = exitcode;
	list->pid = 1;
	return (list);
}

static char	**get_left_side(char **tab, int end)
{
	char	**ret;
	int		i;

	ret = malloc(sizeof(char *) * (end + 1));
	if (!ret)
		return (NULL);
	ret[end] = NULL;
	i = -1;
	while (++i < end)
	{
		ret[i] = ft_strdup(tab[i]);
		if (!ret[i])
		{
			free_tab(ret);
			return (NULL);
		}
	}
	return (ret);
}

static char	**get_right_side(char **tab, int begin)
{
	char	**ret;
	int		i;
	int		size;

	size = tab_len(tab);
	ret = malloc(sizeof(char *) * (size - begin + 1));
	if (!ret)
		return (NULL);
	ret[size - begin] = NULL;
	i = -1;
	while (++i < (size - begin))
	{
		ret[i] = ft_strdup(tab[i + begin]);
		if (!ret[i])
		{
			free_tab(ret);
			return (NULL);
		}
	}
	return (ret);
}

t_group	*parser(char **cmd, char ***env_ptr, int *exitcode)
{
	int		breakpoint;
	char	**right_side;
	t_group	*list;
	
	list = init_group(cmd, env_ptr, exitcode);
	protect_malloc_during_build(cmd, env_ptr, list);
	breakpoint = first_operator(cmd);
	if (breakpoint == -1)
	{
		if (cmd && cmd[0])
			list->cmd = copy_tab(cmd);
		return (list);
	}
 	if (is_control_operator(cmd[0]) && cmd[0][0] != '(')
	{
		list->operator = get_operator(cmd[0]);
		breakpoint = 1;
	}
	else if (cmd[0][0] == '(')
	{
		list->operator = OPEN_SUBSHELL;
		breakpoint = find_closing_parenth(cmd);
	}
	if (list->operator == NONE || list->operator == OPEN_SUBSHELL)
	{
		list->cmd = get_left_side(cmd, breakpoint);
		protect_malloc(list, list->cmd);
/* 		printf("Left side: ");
		for (int i = 0; list->cmd[i]; i++)
			printf("%s --  ", list->cmd[i]); 
		printf("\n"); */
	}
	if (cmd[breakpoint] == NULL)
		return (list);
	right_side = get_right_side(cmd, breakpoint);
	protect_malloc(list, right_side);
/*	int len = tab_len(right_side);
  	printf("Right side: ");
	for (int i = 0; i < len; i++)
		printf("%s -- ", right_side[i]);
	printf("\n"); */
	list->next = parser(right_side, env_ptr, exitcode);
	free_tab(right_side);		//	Causes segfault in subshell. Don't know why.
	if (list->next)
		list->next->previous = list;
	return (list);
}