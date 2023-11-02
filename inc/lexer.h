#ifndef LEXER_H
# define LEXER_H

#include "minishell.h"

/*	lexer_utils.c	*/
bool	is_whitespace(const char c);
bool	is_special_char(const char c);
bool	is_logical_operator(const char a, const char b);
int		wordlen(const char *str, const char c);

/*	tokenizer.c		*/
char	**tokenizer(char const *s);

#endif