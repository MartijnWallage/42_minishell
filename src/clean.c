#include "../inc/minishell.h"

void	cleanup(t_branch *tree)
{
	t_branch	*left;
	t_branch	*right;

	if (tree->left)
		cleanup(tree->left);
	if (tree->right)
		cleanup(tree->right);
	free_tab(tree->cmd);
	free(tree);
}