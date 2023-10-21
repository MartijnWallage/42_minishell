#include "../inc/minishell.h"

char	**lexer(char *line)
{
	char	**tab;

	tab = ft_split(line, ' ');
	return (tab);
}
