/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:46:09 by mwallage          #+#    #+#             */
/*   Updated: 2023/12/10 11:59:25 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	**tokens;
	char	**mini_env;
	int		exitcode;
	t_group	*list;

	if (argc != 1 || argv[1])
		return (1);
	mini_env = copy_tab(envp);
	exitcode = 0;
	while (1)
	{
		line = reader(mini_env);
		if (!line)
		{
			error_msg("readline failed");
			free_tab(mini_env);
			rl_clear_history();
			exit(errno);
		}
		tokens = tokenizer(line);
		free(line);
		if (!tokens)
		{
			rl_clear_history();
			free_tab(mini_env);
			error_msg(MALLOC_MSG);
			return (MALLOC_CODE);
		}
		if (!is_valid_syntax(tokens, &exitcode))
		{
			free_tab(tokens);
			continue ;
		}
 		list = parser(tokens, &mini_env, &exitcode);
 		
/* 		printf("The following is sent to the executor:\n");
		t_group *current = list;
		while (current)
		{
			if (current->cmd)
				for (int j = 0; j < tab_len(current->cmd); j++)
					printf("%s ", current->cmd[j]);
			else
				printf("%d", current->operator);
			current = current->next;
			if (current)
				printf(" -> ");
		}
		printf("\n\n");
		 */
		executor(list);
		cleanup(list);
	}
	return (0);
}
