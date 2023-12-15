/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/15 13:50:01 by jmuller          ###   ########.fr       */
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
	list->subshell = NULL;
	list->env_ptr = env_ptr;
	list->infile = STDIN_FILENO;
	list->heredoc = STDIN_FILENO;
	list->outfile = STDOUT_FILENO;
	list->original_stdin = STDIN_FILENO;
	list->original_stdout = STDOUT_FILENO;
	list->exitcode = exitcode;
	list->pid = -1;
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

int	fill_group(t_group *group, char **cmd, int breakpoint)
{
	if (cmd[0][0] == '(')
	{
		breakpoint = find_closing_parenth(cmd);
		group->operator = OPEN_SUBSHELL;
		group->subshell = parser(&cmd[1], group->env_ptr, group->exitcode);
		if (group->subshell == (void *)-1)
			return (-2);
	}
	else if (cmd[0][0] == ')')
	{
		group->operator = CLOSE_SUBSHELL;
		breakpoint = -1;
	}
	else if (is_control_operator(cmd[0]))
	{
		group->operator = get_operator(cmd[0]);
		breakpoint = 1;
	}
	else
	{
		group->cmd = get_left_side(cmd, breakpoint);
		protect_malloc(group, group->cmd);
	}
	return (breakpoint);
}

/**
 * @brief Parses the syntax-checked 2D-Array of tokens into a linked list of 
 * control group.
 * 
 * Each entry in the resulting list represents a control group, 
 * separated by operators such as '|', '||', '&&', and '('.
 * The function performs the following steps:
 * 1. Initializes the list to store control groups.
 * 2. Locates the first control group by checking for operators.
 * 3. Fills struct of the control group left to the operator
 * 4. Identifies the right-side of the operator for further parsing, i.e.
 *    the left-over bit of the 2D-Array. 
 * 5. Recursively processes the right side of the operator to build 
 * 	  the next list entry.
 * @param cmd Pointer to syntax-checked 2D-Array of tokens (original input).
 * @param env_ptr Tripple pointr to environmental variables (allows editing env).
 * @param exitcode Pointer to an int that stores exit codes.
 * @return Returns a pointer to the head of a linked list of control groups.
 */

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
		if (!parse_heredoc(list))
		{
			cleanup(list);
			return ((void *)-1);
		}
		return (list);
	}
	breakpoint = fill_group(list, cmd, breakpoint);
	if (!parse_heredoc(list))
	{
		cleanup(list);
		return ((void *)-1);
	}
	if (breakpoint == -2)
		return ((void *)-1);
	if (breakpoint == -1 || cmd[breakpoint] == NULL)
		return (list);
	right_side = get_right_side(cmd, breakpoint);
	protect_malloc(list, right_side);
	list->next = parser(right_side, env_ptr, exitcode);
	if (list->next == (void *) -1)
		return ((void *) -1);
	free_tab(right_side);
	if (list->next)
		list->next->previous = list;
	return (list);
}
