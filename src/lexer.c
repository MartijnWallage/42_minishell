#include "../inc/minishell.h"

char	**lexer(char *line)
{
	char	**tab;

	tab = ft_split(line, ' ');
	// take care of quotes etc.
	return (tab);
}
