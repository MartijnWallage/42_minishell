#include "minishell.h"

void	remove_first_char_pair(char *str)
{
	while (str && *str && *(str + 1))
	{
		*str = *(str + 1);
		str++;
	}
	*str = *(str + 1);
}

void	remove_quotes(char *str)
{
	char	opening_quote;

	while (str && *str)
	{
		if (*str == '\'' || *str == '\"')
		{
			opening_quote = *str;
			remove_first_char_pair(str);
			while (*str && *str != opening_quote)
				str++;
			if (*str == opening_quote)
				remove_first_char_pair(str);
		}
		else
			str++; 
	}
}

void	expander(t_group *list)
{
	t_group	*current;
	int		i;

	// expand variables, except when $ occurs within double quotes 
	current = list;
	while (current)
	{
		i = 0;
		while (current->cmd[i])
		{
			remove_quotes(current->cmd[i]);
			i++;
		}
		current = current->next;
	}
}