/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 11:13:16 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/05 10:40:52 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *	Special Characters, we need to handle: ', ", <, >, >>, <<, 
 *	|, $, &?, ||, &&, (, ).
 *	Special Characters, we dont have to handle: \ , ;
 *	Special Characters, that the subject doesnt say anything about: !, #, [, ] 
 */

static int	token_len(const char *s)
{
	int		size;
	char	opening_quote;

	if (is_special_charpair(*s, *(s + 1)))
		return (2);
    if (is_special_char(*s))
		return (1);
	size = 0;
	while (s[size] && !is_whitespace(s[size]) && !is_special_char(s[size]))
	{
		if (is_quotation_mark(s[size]))
		{
			opening_quote = s[size];
			size++;
			while (s[size] && s[size] != opening_quote)
				size++;
			if (s[size] == opening_quote)
				size++;
		}
		else
			size++;
	}
	return (size);
}

static int	count_words(const char *s)
{
	int		counter;

	counter = 0;
	while (*s)
	{
		while (is_whitespace(*s))
			s++;
		if (*s)
		{
			counter++;
			s += token_len(s);
		}
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
		fill_str(tab[i], s, wordlen);
		s += wordlen;
	}
	return (tab);
}
