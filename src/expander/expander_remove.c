/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_remove.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/13 16:21:05 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

int	remove_quotes(char *str)
{
	char	opening_quote;
	int		flag;

	flag = 1;
	while (str && *str)
	{
		if (*str == '\'' || *str == '\"')
		{
			if (*str == '\'')
				flag = 0;
			opening_quote = *str;
			remove_first_char(str);
			while (*str && *str != opening_quote)
				str++;
			remove_first_char(str);
		}
		else
			str++;
	}
	return (flag);
}

void	remove_redirect(char **cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (cmd[i])
	{
		if (is_redirect(cmd[i]))
		{
			remove_word(cmd, i + 1);
			remove_word(cmd, i);
		}
		else
			i++;
	}
}

void	remove_first_char(char *str)
{
	if (!str || !*str)
		return ;
	while (str && *str && *(str + 1))
	{
		*str = *(str + 1);
		str++;
	}
	*str = *(str + 1);
}

void	remove_word(char **tab, int index)
{
	int	i;

	i = index;
	while (tab[i + 1])
	{
		tab[i] = tab[i + 1];
		i++;
	}
	free(tab[i + 1]);
	tab[i] = NULL;
}
