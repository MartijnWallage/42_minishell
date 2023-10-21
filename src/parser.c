#include "../inc/minishell.h"

static t_branch	*init_branch(char **tokens, char **env)
{
	t_branch	*branch;
	
	branch = malloc(sizeof(t_branch));
	// protect malloc
	branch->cmd = tokens;
	branch->env = env;
	branch->operator = 0;
	branch->left = NULL;
	branch->right = NULL;
	branch->child = false;
	branch->lastborn = true;
	return (branch);
}

static char	**init_group(char **tab, int end)
{
	char	**ret;
	int		i;

	ret = malloc(sizeof(char *) * (end + 1));
	// protect malloc
	ret[end] = NULL;
	i = -1;
	while (++i < end)
		ret[i] = ft_strdup(tab[i]);
	return (ret);
}

static int	first_pipe(char **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i])
	{
		if (tokens[i][0] == '|' && tokens[i + 1])
			return (i);
		i++;
	}
	return (-1);
}

t_branch	*parser(char **tokens, char **envp)
{
	t_branch	*tree;
	int			breakpoint;
	int			i;
	size_t		size;

	tree = init_branch(tokens, envp);
	breakpoint = first_pipe(tokens);
	if (breakpoint == -1)
		return (tree);
	tree->lastborn = false;
	pipe(tree->pipefd);
	tree->left = parser(init_group(tokens, breakpoint), envp);
	tree->left->child = true;
	tree->left->lastborn = false;
	size = tab_len((void **)&tokens[breakpoint + 1]);
	tree->right = parser(init_group(&tokens[breakpoint + 1], size), envp);
	tree->right->child = true;
	return (tree);
}
