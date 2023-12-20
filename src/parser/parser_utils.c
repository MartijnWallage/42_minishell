/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/20 16:32:52 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

t_operator	get_operator(char *str)
{
	t_operator	operator;

	if (str == NULL)
		operator = NONE;
	else if (ft_strncmp(str, "|", 2) == 0)
		operator = PIPE;
	else if (ft_strncmp(str, "&&", 3) == 0)
		operator = AND;
	else if (ft_strncmp(str, "||", 3) == 0)
		operator = OR;
	else if (ft_strncmp(str, "(", 2) == 0)
		operator = OPEN_SUBSHELL;
	else if (ft_strncmp(str, ")", 2) == 0)
		operator = CLOSE_SUBSHELL;
	else
		operator = NONE;
	return (operator);
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
			return (free_tab(ret), NULL);
	}
	return (ret);
}
