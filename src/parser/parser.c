/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 14:27:06 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/// @brief	Find first operator in **tokens
/// @param tokens  array of strings passed by the tokenizer
/// @return	Index of the first operator, or -1 if no operator
static int	first_operator(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
	{
		if (is_control_operator(tokens[i]) && tokens[i + 1])
			return (i);
		i++;
	}
	return (-1);
}

/// @brief 			Initialize group
/// @param cmd 		list of tokens passed by tokenizer
/// @param env 		copy of environment
/// @param exitcode last known exitcode
/// @return 		the initalized group
static t_group	*init_group(char **cmd, char **env, int exitcode)
{
	t_group	*list;

	list = malloc(sizeof(t_group));
	if (!list)
		return (NULL);
	list->cmd = cmd;
	list->previous = NULL;
	list->next = NULL;
	list->env = copy_tab(env);
	protect_malloc(list, list->env);
	list->operator = 0;
	list->heredoc = NULL;
	list->infile = STDIN_FILENO;
	list->outfile = STDOUT_FILENO;
	list->infile_name = NULL;
	list->outfile_name = NULL;
	list->original_stdin = dup(STDIN_FILENO);		// Probably should not do this here but more specifically where it is needed
	list->original_stdout = dup(STDOUT_FILENO);
	list->exitcode = exitcode;
	list->pid = 1;
	return (list);
}

t_group	*parser(char **cmd, char **env, int exitcode)
{
	int		breakpoint;
	char	**right_side;
	t_group	*list;

	list = init_group(cmd, env, exitcode);
	if (!list)
	{
		free_tab(cmd);
		free_tab(env);
		error_msg(MALLOC_MSG);
		exit(MALLOC_CODE);
	}
	parse_redirect(list);
	breakpoint = first_operator(cmd);
	if (breakpoint == -1)
		return (list);
	list->cmd = get_left_side(cmd, breakpoint);
	protect_malloc(list, list->cmd);
	if (ft_strncmp(cmd[breakpoint], "|", 2) == 0)
		list->operator = PIPE;
	else if (ft_strncmp(cmd[breakpoint], "&&", 3) == 0)
		list->operator = AND;
	else if (ft_strncmp(cmd[breakpoint], "||", 3) == 0)
		list->operator = OR;
	right_side = get_right_side(cmd, breakpoint + 1);
	free_tab(cmd);
	protect_malloc(list, right_side);
	list->next = parser(right_side, list->env, exitcode);
	protect_malloc(list, list->next);
	list->next->previous = list;
	return (list);
}
