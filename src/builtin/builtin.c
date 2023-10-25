#include "minishell.h"

void	minishell_exit(t_group *group)
{
	cleanup(group);
	exit(0);
}

void	minishell_env(t_group *group)
{
	int	i;

	i = 0;
	while (group->env[i])
	{
		printf("%s\n", group->env[i]);
		i++;
	}
}

void	minishell_pwd(t_group *group)
{
	int	i;

	i = 0;
	while (group->env[i])
	{
		if (ft_strncmp(group->env[i], "PWD", 3) == 0)
		{
			printf("%s\n", &(group->env[i][4]));
			break ;
		}
		i++;
	}
}

void	builtin(t_group	*group)
{
	if (ft_strcmp(group->cmd[0], "exit") == 0)
		minishell_exit(group);
	if (ft_strcmp(group->cmd[0], "env") == 0)
		minishell_env(group);
	if (ft_strcmp(group->cmd[0], "pwd") == 0)
		minishell_pwd(group);
	if (group->operator == PIPE)
	{
		cleanup(group);
		exit(0);
	}
}

bool	is_builtin(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (true);
	if (ft_strcmp(cmd, "cd") == 0)
		return (true);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (true);
	if (ft_strcmp(cmd, "env") == 0)
		return (true);
	if (ft_strcmp(cmd, "export") == 0)
		return (true);
	if (ft_strcmp(cmd, "unset") == 0)
		return (true);
	if (ft_strcmp(cmd, "echo") == 0)
		return (true);
	return (false);
}