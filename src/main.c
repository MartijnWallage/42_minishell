/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/14 09:33:45 by mwallage         ###   ########.fr       */
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
	executor(list);
//	check_signal_flag(list);
	cleanup(list);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**mini_env;
	int		exitcode;

	if (argc != 1 || argv[1])
		return (1);
	mini_env = copy_tab(envp);
	exitcode = 0;
	signal_handler_parent();
	while (1)
	{
		line = reader(mini_env);
		if (line == NULL)
			exit_after_reader(mini_env);
		main_routine(line, &mini_env, &exitcode);
	}
}
