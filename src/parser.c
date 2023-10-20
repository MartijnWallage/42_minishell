#include "../inc/minishell.h"

static t_branch	*init_branch(t_token *command, char **env)
{
	t_branch	*branch;
	
	branch = malloc(sizeof(t_branch));
	// protect malloc
	branch->env = env;
	branch->left = NULL;
	branch->right = NULL;
	branch->infile = STDIN_FILENO;
	branch->outfile = STDOUT_FILENO;
	branch->errfile = STDERR_FILENO;
	branch->command = command;
	branch->child = false;
	return (branch);
}

static int	find_meta(t_token *tokens)
{
	int	i;
	
	i = 0;
	while (tokens[i].last_token == false)
	{
		if (tokens[i].meta)
			return (i);
		i++;
	}
	return (-1);
}
t_branch	*parser(t_token *tokens, char **envp)
{
	t_branch	*tree;
	t_token		*left_tokens;
	t_token		*right_tokens;
	int			i;

	tree = init_branch(tokens, envp);	
	i = find_meta(tokens);
	if (i == -1)
		return (tree);
	left_tokens = token_cpy(tokens, 0, i);
	right_tokens = token_cpy(tokens, i + 1, tokens_len(tokens));
	pipe(tree->pipefd);
	tree->left = parser(left_tokens, envp);
	tree->left->child = true;
	tree->left->outfile = tree->pipefd[1];
	tree->right = parser(right_tokens, envp);
	tree->right->child = true;
	tree->right->infile = tree->pipefd[0];
	return (tree);	
}