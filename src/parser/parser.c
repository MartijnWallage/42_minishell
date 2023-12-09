/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 19:38:06 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/// @brief 			Initialize group
/// @param cmd 		list of tokens passed by tokenizer
/// @param env 		copy of environment
/// @param exitcode last known exitcode
/// @return 		the initalized group
static t_group	*init_group(char **cmd, char ***mini_env, int *exitcode)
{
	t_group	*list;

	list = malloc(sizeof(t_group));
	protect_malloc_during_build(cmd, mini_env, list);
	list->cmd = cmd;
	list->operator = NONE;
	list->previous = NULL;
	list->next = NULL;
	list->mini_env = mini_env;
	list->infile = STDIN_FILENO;
	list->outfile = STDOUT_FILENO;
	list->original_stdin = dup(STDIN_FILENO);		// Probably should not do this here but more specifically where it is needed
	list->original_stdout = dup(STDOUT_FILENO);
	list->exitcode = exitcode;
	list->pid = 1;
	return (list);
}

t_group	*parser(char **cmd, char ***mini_env, int *exitcode)
{
	int		breakpoint;
	char	**right_side;
	t_group	*list;
	
	if (!is_valid_syntax(cmd, exitcode))
		return (free_tab(cmd), NULL);
	list = init_group(cmd, mini_env, exitcode);
	protect_malloc_during_build(cmd, mini_env, list);
	breakpoint = first_operator(cmd);
	if (breakpoint == -1)
		return (list);
	if (cmd[0] && cmd[0][0] == '(')
	{
		breakpoint = find_closing_parenth(cmd);
		list->cmd = get_left_side(cmd, breakpoint + 1);
		list->operator = OPEN_SUBSHELL;
		if (cmd[breakpoint + 1])
		{
			right_side = get_right_side(cmd, breakpoint + 1);
			protect_malloc(list, right_side);
			list->next = parser(right_side, mini_env, exitcode);
			list->next->previous = list;
		}
		free_tab(cmd);
		return (list);
	}
 	if (is_control_operator(cmd[0]))
	{
		list->cmd = NULL;
		list->operator = get_operator(cmd[0]);
		right_side = get_right_side(cmd, 1);
		protect_malloc(list, right_side);
		free_tab(cmd);
		list->next = parser(right_side, mini_env, exitcode);
		if (list->next)
			list->next->previous = list;
		return (list);
	}
	list->cmd = get_left_side(cmd, breakpoint);
	protect_malloc(list, list->cmd);
	right_side = get_right_side(cmd, breakpoint);
	protect_malloc(list, right_side);
//	free_tab(cmd);
	list->next = parser(right_side, mini_env, exitcode);
	if (list->next)
		list->next->previous = list;
	return (list);
}