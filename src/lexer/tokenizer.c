/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 11:13:16 by mwallage          #+#    #+#             */
/*   Updated: 2023/11/02 13:53:04 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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

static int	token_len(const char *s)
{
	int	size;

	if (*s == '\'' || *s == '\"')
		return (wordlen(s + 1, *s));
	if (is_logical_operator(*s, *(s + 1)))
		return (2);
    if (is_special_char(*s))
		return (1);
	size = 0;
	while (s[size] && !is_whitespace(s[size]) 
        && !is_special_char(s[size]))
		size++;
	return (size);
}

static int	count_words(const char *s)
{
	char    lastchar;
	int		counter;

	lastchar = ' ';
	counter = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '\"')
		{
			s += token_len(s);
			s += 2;
			counter++;
			lastchar = ' ';
			continue ;
		}
        if (is_special_char(*s) && !is_logical_operator(lastchar, *s))
            counter++;
        else if ((is_whitespace(lastchar) || is_special_char(lastchar)) 
            && (!is_whitespace(*s) && !is_logical_operator(lastchar, *s)))
            counter++;
		lastchar = *s;
		s++;
	}
	return (counter);
}

static void	fill_str(char *tab, const char *s, int wordlen)
{
	int	i;

	tab[wordlen] = '\0';
	i = -1;
	while (++i < wordlen)
	{
		tab[i] = *s;
		s++;
	}
}

char	**tokenizer(char const *s)
{
	char	**tab;
	int		words;
	int		wordlen;
	int		i;

	words = count_words(s);
	tab = malloc((words + 1) * sizeof(char *));
	if (tab == NULL)
		return (NULL);
    tab[words] = NULL;
	i = -1;
	while (++i < words)
	{
		while (is_whitespace(*s))
			s++;
		wordlen = token_len(s);
		tab[i] = malloc(wordlen + 1);
		if (tab[i] == NULL)
			return (free_tab(tab));
		s += (*s == '\'' || *s == '\"');
		fill_str(tab[i], s, wordlen);
		s += wordlen;
		s += (*s == '\'' || *s == '\"');
	}
	return (tab);
}
