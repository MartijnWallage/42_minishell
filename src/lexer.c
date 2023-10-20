#include “../inc/minishell.h”

t_token	*lexer(char *line)
{
	char	**tab;
	t_token	*tokens;
	int		size;
	char	*name;
	int		i;
	
	tab = ft_split(line, ‘ ‘);
	size = tab_size(tab);
	tokens = malloc(sizeof(t_token) * size + 1);
	if (!tokens)
		handle_error(MALLOC_MSG, MALLOC_CODE);
	i = -1;
	while (++i < size)
	{
		name = ft_strdup(tab[i]);
		if (!name)
		{
			free(tokens);
			free_tab(tab);
			handle_error(MALLOC_MSG, MALLOC_CODE);
		}
		tokens[i].word = NULL;
		tokens[i].meta = NULL;
		tokens[i].metameta = NULL;
		tokens[i].last_token = false;
		if (ft_strcmp(tab[i], ‘|’) == 0)
			tokens[i].meta = name;
		else {
			tokens[i].word = name;
		}
	}
	tokens[i - 1].last_token = true;
	free_tab(tab);
	return (tokens);
}