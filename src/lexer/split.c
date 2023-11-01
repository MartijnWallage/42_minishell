/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 11:13:16 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/01 16:51:14 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Reminder:
Handle ’ (single quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence.

• Handle " (double quote) which should prevent the shell from interpreting the meta-
characters in the quoted sequence except for $ (dollar sign).
• Special Characters, we need to handle: ','', <, >, >>, <<, |, $, &?, ||, &&, (, ), 
• Special Characters, we dont have to handle: \ , ;
• Special Characters, that the subject doesnt say anything about: !, *, ?, #, [, ] 

*/

bool    is_whitespace(char c)
{
    if (c == ' ')
        return (true);
    if (c == '\t')
        return (true);
    if (c == '\v')
        return (true);
    if (c == '\n')
        return (true);
    return (false);
}

bool    is_special_char(char c)
{
    if (c == '|')
        return (true);
    if (c == '<')
        return (true);
    if (c == '>')
        return (true);
    if (c == '(')
        return (true);
    if (c == ')')
        return (true);
    if (c == '&')
        return (true);
    return (false);
}

static size_t	ft_wordlen(const char *s)
{
	size_t	size;

    if (is_special_char(*s))
        return (1);
	size = 0;
	while (s[size] && !is_whitespace(s[size]) 
        && !is_special_char(s[size]))
		size++;
	return (size);
}

static size_t	count_words(const char *s)
{
	char    lastchar;
	size_t	counter;

	lastchar = ' ';
	counter = 0;
	while (*s)
	{
        if (is_special_char(*s))
            counter++;
        else if ((is_whitespace(lastchar) || is_special_char(lastchar)) 
            && !is_whitespace(*s))
            counter++;
	    // We still have to take care of: ||, &&
		lastchar = *s;
		s++;
	}
	return (counter);
}

static void	fill_str(char *tabs, const char *s, size_t wordlen)
{
	size_t	i;

	i = 0;
	while (i < wordlen - 1)
	{
		tabs[i] = *s;
		i++;
		s++;
	}
	tabs[i] = '\0';
}

char	**token_split(char const *s)
{
	char	**tab;
	size_t	words;
	size_t	wordlen;
	size_t	i;

	words = count_words(s);
	tab = malloc((words + 1) * sizeof(char *));
	if (tab == NULL)
		return (NULL);
    tab[words] = NULL;
	i = 0;
	while (i < words)
	{
		while (is_whitespace(*s))
			s++;
		wordlen = ft_wordlen(s);
		tab[i] = malloc(wordlen + 1);
		if (tab[i] == NULL)
        {
			free_tab(tab);
            return (NULL);
        }
		fill_str(tab[i], s, wordlen + 1);
		s += wordlen;
		i++;
	}
	return (tab);
}
