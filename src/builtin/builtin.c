#include "minishell.h"

/*	We have to exit without using exit()	*/
void	minishell_exit()
{
	exit(0);
}

void	builtin(char **cmd, char **env)
{
	if (ft_strcmp(cmd[0], "exit") == 0)
		minishell_exit();
}

bool	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	return (false);
}
