/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/05 17:31:33 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	branch(t_branch	*branch)
{
	/* 
	 	Make a stack for parentheses and quotes
		If they don't match, readline("\t> ") for more input.
		If | or && or || occurs freely (outside quotes), branch left and right.
			Else if there are no more pipes or logical operators, 
			take out redirections (save in struct), expand variables
			and return current branch.
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

char	**genv;

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
		branch(tree);
		read_tree(tree); */
		simple_command(line); // this will later be called after descending the tree
		free(line);
	}
	return (0);
}
