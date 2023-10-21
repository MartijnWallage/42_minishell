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

void	simple_command(t_branch *branch)
{
	pid_t	pid;
	
 	if (branch->child)
		exec(branch->cmd, branch->env);
	else 
	{
		pid = fork();
		// protect fork
		if (pid == 0)
			exec(branch->cmd, branch->env);
		else
			waitpid(pid, NULL, 0);
	}
}

void	executor(t_branch *branch)
{
	pid_t	pid;
	pid_t	pid2;

	if (!branch->left || !branch->right)
		simple_command(branch);
	else 
	{
		pid = fork();
		// protect fork
		if (pid == 0)
		{
			close(branch->pipefd[0]);
			dup2(branch->pipefd[1], STDOUT_FILENO);
			close(branch->pipefd[1]);
			executor(branch->left);
		}
		waitpid(pid, NULL, 0);
		close(branch->pipefd[1]);
		pid2 = fork();
		if (pid2 == 0)
		{
			dup2(branch->pipefd[0], STDIN_FILENO);
			close(branch->pipefd[0]);
			executor(branch->right);
		}
		close(branch->pipefd[0]);
		waitpid(pid2, NULL, 0);
	}
}