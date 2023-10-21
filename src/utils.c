#include "../inc/minishell.h"

size_t	tab_len(void **tab)
{
	size_t	size;
	
	size = 0;
	while (tab && tab[size])
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