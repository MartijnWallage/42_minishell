#include “../inc/minishell.h”

void	cleanup(t_branch *tree)
{
	free(tree);
}