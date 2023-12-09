/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/22 18:15:14 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 15:30:17 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	find_closing_parenth(char **cmd)
{
	int	i;
	int	counter;
	
	i = 1;
	counter = 1;
	while (counter)
	{
		counter += (cmd[i][0] == '(');
		counter -= (cmd[i][0] == ')');
		i++;
	}
	return (i - 1);
}

/// @brief	Find first operator in **tokens
/// @param tokens  array of strings passed by the tokenizer
/// @return	Index of the first operator, or -1 if no operator
int	first_operator(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
	{
		if (is_control_operator(tokens[i]))
			return (i);
		i++;
	}
	return (-1);
}

char	**get_left_side(char **tab, int end)
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

char	**get_right_side(char **tab, int begin)
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

t_operator	get_operator(char *str)
{
	t_operator	operator;

	if (ft_strncmp(str, "|", 2) == 0)
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