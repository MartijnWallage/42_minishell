/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:08:00 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/09 20:25:15 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

int	remove_quotes(char *str)
{
	char	OPEN_SUBSHELLing_quote;
	int		flag;

	flag = 1;
	while (str && *str)
	{
		if (*str == '\'' || *str == '\"')
		{
			if (*str == '\'')
				flag = 0;
			OPEN_SUBSHELLing_quote = *str;
			remove_first_char(str);
			while (*str && *str != OPEN_SUBSHELLing_quote)
				str++;
			remove_first_char(str);
		}
		else
			str++;
	}
	return (flag);
}

int	get_keylen(char *word)
{
	int	keylen;

	if (word[0] == '?')
		return (1);
	keylen = 0;
	while (ft_isalnum(word[keylen]))
		keylen++;
	return (keylen);
}

char	*get_value(t_group *group, char *word)
{
	int		keylen;
	char	*value;
	char	*key;

	if (word[0] == '?')
	{
		value = ft_itoa(*group->exitcode);
		protect_malloc(group, value);
		return (value);
	}
	keylen = get_keylen(word);
	key = ft_substr(word, 0, keylen);
	protect_malloc(group, key);
	value = mini_getenv(*group->env_ptr, key);
	free(key);
	if (value)
	{
		value = ft_strdup(value);
		protect_malloc(group, value);
	}
	return (value);
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
