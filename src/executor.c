#include "../inc/minishell.h"


void	exec(char *cmd, char **env)
{
	char	*path;
	char	**whole_cmd;

	whole_cmd = ft_split(cmd->, ' ');
	path = get_path(whole_cmd[0], env);
	if (execve(path, whole_cmd, env) == -1)
	{
		if (ft_strcmp(path, whole_cmd[0]))
			free(path);
		free_tab(whole_cmd);
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd, 2);
		exit(errno);
	}
}

void	simple_command(t_branch *branch)
{
	pid_t	pid;
	
	if (branch->child)
		exec(branch->command);
	else {
		pid = fork();
		// protect fork
		if (pid == 0)
			exec(branch->command);
	}
}

void	executor(t_branch *tree)
{
	pid_t	pid;
	pid_t	pid2;

	if (!branch->left || !branch->right)
		simple_command(branch);
	pid = fork();
	// protect fork
	if (pid == 0)
	{
		close(branch->pipefd[0]);
		dup2(branch->pipefd[1], STDOUT_FILENO);
		close(branch->pipefd[1]);
		read_tree(branch->left);
	}
	else
	{
		pid2 = fork();
		if (pid2 == 0)
		{
			close(branch->pipefd[1]);
			dup2(branch->pipefd[0], STDIN_FILENO);
			close(branch->pipefd[0]);
			read_tree(branch->right);
		}
	}
}