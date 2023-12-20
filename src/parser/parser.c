/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/20 16:30:45 by mwallage         ###   ########.fr       */
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
	t_group	*group;

	group = malloc(sizeof(t_group));
	if (group == NULL)
		return (NULL);
	group->env_ptr = env_ptr;
	group->cmd = NULL;
	group->operator = get_operator(*cmd);
	group->previous = NULL;
	group->next = NULL;
	group->subshell = NULL;
	group->infile = STDIN_FILENO;
	group->heredoc = STDIN_FILENO;
	group->outfile = STDOUT_FILENO;
	group->original_stdin = STDIN_FILENO;
	group->original_stdout = STDOUT_FILENO;
	group->exitcode = exitcode;
	group->pid = -1;
	return (group);
}

t_group	*parser(char **cmd, char ***env_ptr, int *exitcode)
{
	t_group	*group;

	group = init_group(cmd, env_ptr, exitcode);
	if (group == NULL)
		return (NULL);
	if (group->operator == NONE)
	{
		group->cmd = copy_tab_until_operator(cmd);
		if (group->cmd == NULL)
			return (cleanup(group), NULL);
	}
	else if (group->operator == OPEN_SUBSHELL)
	{
		group->subshell = parser(&cmd[1], env_ptr, exitcode);
		if (group->subshell == NULL)
			return (cleanup(group), NULL);
	}
	cmd = get_next_cmd(group, cmd);
	if (cmd == NULL || *cmd == NULL)
		return (group);
	group->next = parser(cmd, env_ptr, exitcode);
	if (group->next == NULL)
		return (cleanup(group), NULL);
	group->next->previous = group;
	return (group);
}
