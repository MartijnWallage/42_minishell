/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:44:55 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/05 17:37:03 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

static void	exec(char *cmd, char **env, int exitcode)
{
	char	*path;
	char	**whole_cmd;

	whole_cmd = ft_split(cmd, ' ');
	path = get_path(whole_cmd[0], env);
	if (execve(path, whole_cmd, env) == -1)
	{
		if (ft_strcmp(path, whole_cmd[0]))
			free(path);
		free_tab(whole_cmd);
		ft_putstr_fd("pipex: command not found: ", 2);
		ft_putendl_fd(cmd, 2);
		exit(exitcode);
	}
}

static void	child(char *cmd, char **env)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		handle_error("pipe error", 1);
	pid = fork();
	if (pid == -1)
		handle_error("pid error", 1);
	if (pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exec(cmd, env, 0);
	}
	waitpid(pid, NULL, 0);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

static void	write_heredoc(char *delimiter, int pipefd[2])
{
	char	*line;

	close(pipefd[0]);
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			handle_error("incomplete here_doc", 1);
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			exit(0);
		}
		ft_putstr_fd(line, pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
}

static int	get_input(char **av)
{
	int		pipefd[2];
	int		infile;
	pid_t	pid;

	if (ft_strcmp(av[0], "here_doc") == 0)
	{
		if (pipe(pipefd) == -1)
			handle_error("pipe error", 1);
		pid = fork();
		if (pid == -1)
			handle_error("pid error", 1);
		if (pid == 0)
			write_heredoc(av[1], pipefd);
		waitpid(pid, NULL, 0);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	infile = open(av[0], O_RDONLY, 0777);
	if (infile == -1)
		handle_error(av[0], 0);
	dup2(infile, STDIN_FILENO);
	close(infile);
	return (0);
}

int	pipex(int ac, char **av, char **env)
{
	int		outfile;
	int		i;

	check_format(ac, av);
	i = get_input(av);
	while (++i < ac - 1)
		child(av[i], env);
	if (ft_strcmp(av[0], "here_doc") == 0)
		outfile = open(av[ac], O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		outfile = open(av[ac], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (outfile == -1)
		handle_error(av[ac], 1);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	exec(av[ac - 1], env, 127);
	return (0);
}
