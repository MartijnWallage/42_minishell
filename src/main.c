/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/09 17:26:25 by mwallage         ###   ########.fr       */
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

void	grow_tree(t_branch	*branch)
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
	grow_tree(branch->left);
	space = (*(s + 1) == ' ');
	right = init_str(s + 1 + space, ft_strlen(s) - space);
	branch->right = init_branch(right);
	grow_tree(branch->right);
}

void	read_tree(t_branch *branch)
{
	pid_t	pid;

	/* if (branch->operator == PIPE){}; 	//	create two child processes
	 * else if (branch->operator == AND){};	//	do not create child processes.
	 * else if (branch->operator == OR){}; 	//	do not create child processes
	 */
	if (!branch->left || !branch->right)
		simple_command(branch);
	if (pipe(branch->pipefd) == -1)
		handle_error(PIPE_ERR, 1);
	pid = fork();
	if (pid == -1)
		handle_error(FORK_ERR, 1);
	if (pid == 0)
	{
		dup2(branch->pipefd[1], STDOUT_FILENO);
		close(branch->pipefd[0]);
		close(branch->pipefd[1]);
		read_tree(branch->left);
	}
	waitpid(pid, NULL, 0);
	dup2(branch->pipefd[0], STDIN_FILENO);
	close(branch->pipefd[0]);
	close(branch->pipefd[1]);
	read_tree(branch->right);
}

void	simple_command(t_branch *branch)
{
	exec(branch->command, genv);
}

t_branch	*init_branch(char *command)
{
	t_branch	*branch;
	
	branch = malloc(sizeof(t_branch));
	// if malloc fails, throw error
	branch->left = NULL;
	branch->right = NULL;
	branch->infile = STDIN_FILENO;
	branch->outfile = STDOUT_FILENO;
	branch->errfile = STDERR_FILENO;
	branch->connector = 0;
	branch->command = command;
	return (branch);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_branch	*tree;
	pid_t		pid;

	if (argc != 1)
		return (1);
	argv++;	// compiler flags require argv to be used
	genv = envp;
	while (1)
	{
		line = readline(PROMPT);
		if (!is_valid_input(line))
			continue ;
 		tree = init_branch(line);
		grow_tree(tree);
		pid = fork();
		if (pid == -1)
			handle_error(FORK_ERR, 1);
		if (pid == 0)
			read_tree(tree);
		waitpid(pid, NULL, 0);
//		free_tree(tree);
	}
	return (0);
}
