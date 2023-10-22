#include "../inc/minishell.h"

void	exec(char **cmd, char **env)
{
	char	*path;

	path = get_path(cmd[0], env);
	if (execve(path, cmd, env) == -1)
	{
		if (ft_strcmp(path, cmd[0]))
			free(path);
		free_tab(cmd);
		ft_putstr_fd("philoshell: command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		exit(errno);
	}
}

void	simple_command(t_group *group)
{
	pid_t	pid;
	
  	pid = fork();
	if (pid == 0)
		exec(group->cmd, group->env);
	waitpid(pid, NULL, 0);
}

static void	child(t_group *group, char **env)
{
	pid_t	pid;

	if (pipe(group->pipefd) == -1)
		handle_error("pipe error", 1);
	pid = fork();
	if (pid == -1)
		handle_error("pid error", 1);
	if (pid == 0)
	{
		if (group->previous)
		{
			dup2(group->previous->pipefd[0], STDIN_FILENO);
			close(group->previous->pipefd[0]);
		}
		close(group->pipefd[0]);
		if (group->next)
			dup2(group->pipefd[1], STDOUT_FILENO);
		close(group->pipefd[1]);
		exec(group->cmd, env);
	}
	close(group->pipefd[1]);
	waitpid(pid, NULL, 0);
	if (!group->next)
		close(group->pipefd[0]);
}

void	executor(t_group *group)
{
	t_group	*current;
	pid_t	pid;

	if (group->operator == 0)
	{
		simple_command(group);
		return ;
	}
	current = group;
	while (current)
	{
		child(current, current->env);
		current = current->next;
	}
}