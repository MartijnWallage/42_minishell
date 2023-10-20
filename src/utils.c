#include “../inc/minishell.h”

t_token	*token_cpy(t_token *tokens, int begin, int end)
{
	t_token	*cpy;
	int		i;
	
	cpy = malloc(sizeof(t_token) * (end - begin));
	i = -1;
	while (++i < (end - begin))
	{
		cpy[i].word = tokens[i].word;
		cpy[i].meta = tokens[i].meta;
		cpy[i].metameta = tokens[i].metameta;
		cpy[i].last_token = false;
	}
	cpy[i - 1].last_token = true;
	return (cpy);
}

size_t	tokens_len(t_token *tokens)
{
	size_t	size;
	
	size = 0;
	while (tokens[i].last_token == false)
		size++;
	return (size);
}

size_t	tab_len(void **tab)
{
	size_t	size;
	
	size = 0;
	while (tab[size])
		size++;
	return (size);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}