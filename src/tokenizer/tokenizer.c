/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmuller <jmuller@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 11:13:16 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/15 13:01:44 by jmuller          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_token(const char *token)
{
	if (token[0] == '|' && token[1] == '|')
		return (2);
	else if (token[0] == '|')
		return (1);
	else if (token[0] == '&' && token[1] == '&')
		return (2);
	else if (token[0] == '&')
		return (1);
	else if (token[0] == '<' && token[1] == '<')
		return (2);
	else if (token[0] == '<')
		return (1);
	else if (token[0] == '>' && token[1] == '>')
		return (2);
	else if (token[0] == '>')
		return (1);
	else if (token[0] == '(')
		return (1);
	else if (token[0] == ')')
		return (1);
	return (0);
}

static int	token_len(const char *s)
{
	int		size;
	char	opening_quote;
	int		token_len;

	token_len = is_token(s);
	if (token_len)
		return (token_len);
	size = 0;
	while (s[size] && !is_meta_char(s[size]))
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

/*
 * @brief Splits string from readline and produces a 2D array of words.
 *
 * @param The Input string produced by readline.
 * @return Returns a pointer to a 2D array of words. Each word being a 
 * null-terminated string. Returns Null if memory allocation fails or input
 * string is NULL.
 */

char	**tokenizer(const char *s)
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
