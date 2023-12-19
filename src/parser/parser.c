/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/19 15:51:10 by mwallage         ###   ########.fr       */
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

char	**after_closing_parenth(char **cmd)
{
	int	counter;

	counter = 1;
	while (++cmd && counter)
	{
		counter += (*cmd[0] == '(');
		counter -= (*cmd[0] == ')');
	}
	return (cmd);
}

char	**get_next_operator(char **cmd)
{
	while (cmd && *cmd && !is_control_operator(*cmd))
		cmd++;
	return (cmd);
}

char	**get_next_cmd(t_group *group, char **cmd)
{
	if (group->operator == NONE)
		return (get_next_operator(cmd));
	else if (group->operator == OPEN_SUBSHELL)
		return (after_closing_parenth(cmd));
	else if (group->operator == CLOSE_SUBSHELL)
		return (NULL);
	else
		return (&(cmd[1]));
}

char	**copy_tab_until_operator(char **tab)
{
	int		size;
	int		i;
	char	**ret;

	if (tab == NULL)
		return (NULL);
	size = 0;
	while (tab[size] && !is_control_operator(tab[size]))
		size++;
	ret = malloc((sizeof(char *)) * (size + 1));
	if (!ret)
		return (NULL);
	ret[size] = NULL;
	i = -1;
	while (++i < size)
	{
		ret[i] = ft_strdup(tab[i]);
		if (!ret[i])
		{
			free_tab(tab);
			return (NULL);
		}
	}
	return (ret);
}

t_group	*parser(char **cmd, char ***env_ptr, int *exitcode)
{
	char	**next_cmd;
	t_group	*group;

	group = init_group(cmd, env_ptr, exitcode);
	if (group == NULL)
		return (error_msg(MALLOC_MSG), NULL);		
	if (group->operator == NONE)
	{
		group->cmd = copy_tab_until_operator(cmd);
		if (group->cmd == NULL)
			return (error_msg(MALLOC_MSG), free(group), NULL);
		parse_heredoc(group);
	}
	else if (group->operator == OPEN_SUBSHELL)
	{
		group->subshell = parser(&cmd[1], env_ptr, exitcode);
		if (group->subshell == NULL)
			return (error_msg(MALLOC_MSG), free(group), NULL);
	}
	next_cmd = get_next_cmd(group, cmd);
	if (next_cmd == NULL || *next_cmd == NULL)
		return (group);
	group->next = parser(next_cmd, env_ptr, exitcode);
	if (group->next == NULL)
		return (error_msg(MALLOC_MSG), cleanup(group->subshell), free_tab(group->cmd), free(group), NULL);
	group->next->previous = group;
	return (group);
}
