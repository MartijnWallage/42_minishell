/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/09 14:52:04 by mwallage         ###   ########.fr       */
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

void	init_branch(t_branch *branch, char *command)
{
	branch = malloc(sizeof(t_branch));
	// if malloc fails, throw error
	branch->left = NULL;
	branch->right = NULL;
	branch->connector = 0;
	branch->command = command;
}

t_branch	*branch(t_branch	*branch)
{
	char	*s;
	char	*left;
	char	*right

	//	Make a stack for parantheses and quotes
	//		If they don't match, readline for more input.
	//	Find first free occurances of |, &&, or ||.
	s = ft_strchr(branch->command, '|');
	if (s == NULL)
		return (branch);	// First take out redirections and expand variables
	branch->connector = PIPE;
	right = branch->command
	init_branch(branch->left, )
	/* 
		branch->connector = connector; // | or && or ||.
		init_branch(branch->left, left_side);
		branch(branch->left);
		init_branch(branch->right, right_side);
		branch(branch->right); 
		*/
}

void	read_tree(t_branch *branch)
{
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


int	main(int argc, char **argv, char **envp)
{
	char		*line;
//	t_branch	*tree;

	if (argc != 1)
		return (1);
	argv++;	// compiler flags require argv to be used
	genv = envp;
	while (1)
	{
		line = readline(PROMPT);
		if (!is_valid_input(line))
			continue ;
/* 		init_branch(tree, line);
		tree->command = line; 
		tree = branch(tree);
		read_tree(tree); */
		simple_command(line); // this will later be called after descending the tree
		free(line);
	}
	return (0);
}
