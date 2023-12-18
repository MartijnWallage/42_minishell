/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/18 22:11:03 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_after_tokenizer(char **mini_env)
{
	rl_clear_history();
	free_tab(mini_env);
	error_msg(MALLOC_MSG);
	exit(MALLOC_CODE);
}

static void	exit_after_reader(char **mini_env)
{
	rl_clear_history();
	free_tab(mini_env);
	printf("exit\n");
	exit(0);
}

/**
 * @brief Executes the main routine by processing a line of input 
 * and managing the command execution lifecycle.
 * 
 * This function takes a line of input, tokenizes it into a 2D array, 
 * and performs several steps:
 * 1. Tokenizes the input line into a 2D array.
 * 2. Performs a syntax check on the tokens.
 * 3. Parses the tokens into a structured format (e.g., a command group).
 * 4. Executes the parsed commands.
 * 5. Handles signals and performs cleanup.
 * @param line The input string from STDIN.
 * @param mini_env Pointer to the array of environment variables.
 * @param exitcode Pointer to an integer representing exit codes.
 * @return This function does not return a value. 
 * It returns early if the syntax is not valid.
 */

static void	main_routine(char *line, char ***mini_env, int *exitcode)
{
	t_group	*list;
	char	**tokens;

	tokens = tokenizer(line);
	free(line);
	if (!tokens)
		exit_after_tokenizer(*mini_env);
	if (!is_valid_syntax(tokens, exitcode))
	{
		free_tab(tokens);
		return ;
	}
	list = parser(tokens, mini_env, exitcode);
	free_tab(tokens);
	if (list == NULL)
	{
		free_tab(*mini_env);
		exit(MALLOC_CODE);
	}
 /* 	t_group *current = list;
	while (current)
	{
		if (current->cmd == NULL)
			printf("Operator %d", current->operator);
		else
			for (int i = 0; i < tab_len(current->cmd); i++)
				printf("%s -- ", current->cmd[i]);
		if (current->subshell)
		{
			t_group	*current2 = current->subshell;
			while (current2)
			{
				if (current->cmd == NULL)
					printf("Subshell Operator %d", current->operator);
				else
					for (int i = 0; i < tab_len(current->cmd); i++)
						printf("subshell %s -- ", current->cmd[i]);
				current2 = current2->next;
			}
		}
		printf("\n");
		current = current->next;
	} */
	executor(list);
	check_signal_flag(list);
	cleanup(list);
}

/**
 * @brief Starts the main program loop which generates 
 * a command line prompt and processes user input.
 *
 * 
 * This function initializes the environment, 
 * continuously reads user input, and passes it to the main routine.
 * It exits if the program is run with any arguments or if an error 
 * occurs during input reading.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param envp The array of environment variables.
 * @return Returns 1 if the program is run with any arguments, 
 * otherwise it runs indefinitely until an exit condition within the loop is met.
 */

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**mini_env;
	int		exitcode;

	if (argc != 1 || argv[1])
		return (1);
	mini_env = copy_tab(envp);
	if (mini_env == NULL)
		return (error_msg(MALLOC_MSG), MALLOC_CODE);
	exitcode = 0;
	while (1)
	{
		line = reader(mini_env);
		if (line == NULL)
			exit_after_reader(mini_env);
		main_routine(line, &mini_env, &exitcode);
	}
}
