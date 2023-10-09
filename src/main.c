/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/09 15:44:18 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**genv;

static int	is_valid_input(char *input)
{
	if (!input)
		return (0);
	if (*input == '?') // just to test for now
	{
		ft_printf("%s\n", INVALID_INPUT);
		free(input);
		return (0);
	}
	return (1);
}

char	*init_str(const char *origin, size_t size)
{
	char	*ret;

	ret = malloc(size);
	// protect malloc
	ft_strlcpy(ret, origin, size);
	return (ret);
}

void	do_branch(t_branch	*branch)
{
	char	*s;
	char	*left;
	char	*right;
	int		space;

	//	Make a stack for parantheses and quotes
	//		If they don't match, readline for more input.
	//	Find first free occurances of |, &&, or ||.
	s = ft_strchr(branch->command, '|');
	if (s == NULL)
		return ;	// First take out redirections and expand variables
	branch->connector = PIPE;
	left = init_str(branch->command, s - branch->command + 1);
	branch->left = init_branch(left);
	do_branch(branch->left);
	space = (*(s + 1) == ' ');
	right = init_str(s + 1 + space, ft_strlen(s) - space);
	branch->right = init_branch(right);
	do_branch(branch->right);
}

void	read_tree(t_branch *branch)
{
	if (branch->left && branch->right)
	{
		// if (branch->operator == PIPE){};
		// else if (branch->operator == AND){};
		// else if (branch->operator == OR){};
		pipe_left(branch->left);
		pipe_right(branch->right);
	}
	else
		simple_command(branch->command);
	/* 
	 * if (branch->left) {
	 * 	if (branch->operator == '|')
	 *		pipe(branch);   // In this function we'd have to call simple_command
	 *	else if (branch->operator == '&&')
	 *		conjunct(branch);  // same
	 *	else if (branch->operator == '||')
	 *		disjunct(branch); // same
	 *  else
	 *		simple_command(branch->command);
	*/
}


void	simple_command(char *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		exec(cmd, genv);
	else // if !(command ends on &)
		waitpid(pid, NULL, 0);
}

t_branch	*init_branch(char *command)
{
	t_branch	*branch;
	
	branch = malloc(sizeof(t_branch));
	// if malloc fails, throw error
	branch->left = NULL;
	branch->right = NULL;
	branch->connector = 0;
	branch->command = command;
	return (branch);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_branch	*tree;

	if (argc != 1)
		return (1);
	argv++;	// compiler flags require argv to be used
	genv = envp;
	while (1)
	{
		line = readline(PROMPT);
		if (!is_valid_input(line))
			continue ;
 		tree = init_branch(ft_strdup(line));
		do_branch(tree);
//		read_tree(tree); */
		simple_command(line); // this will later be called after descending the tree
//		free_tree(tree);
		free(line);
	}
	return (0);
}
