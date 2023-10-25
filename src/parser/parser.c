/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/22 18:15:15 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_left_side(char **tab, int end)
{
	char	**ret;
	int		i;

	ret = malloc(sizeof(char *) * (end + 1));
	// protect malloc
	ret[end] = NULL;
	i = -1;
	while (++i < end)
		ret[i] = ft_strdup(tab[i]);
	return (ret);
}

static char	**get_right_side(char **tab, int begin)
{
	char	**ret;
	int		i;
	size_t	size;

	size = tab_len((void **)tab);
	ret = malloc(sizeof(char *) * (size - begin + 1));
	// protect malloc
	ret[size - begin] = NULL;
	i = -1;
	while (++i < (size - begin))
		ret[i] = ft_strdup(tab[i + begin]);
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

t_group	*parser(char **cmd, char **env)
{
	t_group	*list;
	int		breakpoint;

	list = malloc(sizeof(t_group));
	// protect malloc
	list->cmd = cmd;
	list->previous = NULL;
	list->next = NULL;
	list->env = env;
	list->operator = 0;
	list->infd = STDIN_FILENO;
	list->outfd = STDOUT_FILENO;
	breakpoint = first_pipe(cmd);
	if (breakpoint == -1)
		return (list);
	list->cmd = get_left_side(cmd, breakpoint);
	list->operator = PIPE;
	list->next = parser(get_right_side(cmd, breakpoint + 1), env);
	free_tab(cmd);
	list->next->previous = list;
	list->next->operator = PIPE;
	return (list);
}