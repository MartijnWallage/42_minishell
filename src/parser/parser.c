/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/24 15:01:34 by mwallage         ###   ########.fr       */
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

static int	first_pipe(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i + 1])
			return (i);
		i++;
	}
	return (-1);
}

t_group	*init_group(char **cmd, char **env, int exitcode)
{
	t_group	*list;

	list = malloc(sizeof(t_group));
	if (!list)
		return (NULL);
	list->cmd = cmd;
	list->previous = NULL;
	list->next = NULL;
	list->env = env;
	list->operator = 0;
	list->heredoc = NULL;
	list->infile = STDIN_FILENO;
	list->outfile = STDOUT_FILENO;
	list->infile_name = NULL;
	list->outfile_name = NULL;
	list->original_stdin = STDIN_FILENO;
	list->original_stdout = STDOUT_FILENO;
	list->exitcode = exitcode;
	return (list);
}

t_group	*parser(char **cmd, char **env, int exitcode)
{
	int		breakpoint;
	char	**right_side;
	t_group	*list;

	list = init_group(cmd, env, exitcode);
	if (!list)
		return (NULL);
	parse_redirect(list);
	breakpoint = first_pipe(cmd);
	if (breakpoint == -1)
		return (list);
	list->cmd = get_left_side(cmd, breakpoint);
	if (!list->cmd)
	{
		cleanup(list);
		return (NULL);
	}
	list->operator = PIPE;
	right_side = get_right_side(cmd, breakpoint + 1);
	if (!right_side)
	{
		cleanup(list);
		return (NULL);
	}
	list->next = parser(right_side, env, exitcode);
	if (!list->next)
	{
		cleanup(list);
		return (NULL);
	}
	free_tab(cmd);
	list->next->previous = list;
	list->next->operator = PIPE;
	return (list);
}
