/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:44:55 by mwallage          #+#    #+#             */
/*   Updated: 2023/10/05 17:29:32 by mwallage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	handle_error(char *info, int exitcode)
{
	char	*errno_readable;
	int		len;

	errno_readable = strerror(errno);
	len = ft_strlen(errno_readable);
	write(2, "pipex: ", 7);
	write(2, errno_readable, len);
	len = ft_strlen(info);
	write(2, ": ", 2);
	write(2, info, len);
	write(2, "\n", 1);
	exit(exitcode);
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

static char	*try_paths(char *cmd, char **paths)
{
	int		i;
	char	*whole_cmd;
	char	*temp;

	i = 0;
	while (paths[i])
	{
		whole_cmd = ft_strjoin(paths[i], "/");
		temp = whole_cmd;
		whole_cmd = ft_strjoin(whole_cmd, cmd);
		if (temp != whole_cmd)
			free(temp);
		if (access(whole_cmd, F_OK | X_OK) == 0)
			return (whole_cmd);
		free(whole_cmd);
		i++;
	}
	return (cmd);
}

char	*get_path(char *cmd, char **env)
{
	char	**line;
	char	**paths;
	char	*whole_cmd;
	int		i;

	if (!env)
		return (cmd);
	i = 0;
	while (1)
	{
		if (!env[i])
			return (cmd);
		line = ft_split(env[i++], '=');
		if (ft_strncmp(line[0], "PATH", 4) == 0)
			break ;
		free_tab(line);
	}
	paths = ft_split(line[1], ':');
	free_tab(line);
	whole_cmd = try_paths(cmd, paths);
	free_tab(paths);
	return (whole_cmd);
}

void	check_format(int ac, char **av)
{
	if (ac < 4)
	{
		ft_putstr_fd("Format:\n", 2);
		ft_putstr_fd("\tinfile cmd1 cmd2 ... outfile\n", 2);
		ft_putstr_fd(
			"\there_doc DELIMITER cmd1 cmd2 ... outfile\n", 2);
		exit(1);
	}
	if ((ft_strcmp(av[0], "here_doc") == 0) && ac < 5)
	{
		ft_putstr_fd("Format:\n", 2);
		ft_putstr_fd(
			"\there_doc DELIMITER cmd1 cmd2 ... outfile\n", 2);
		exit(1);
	}
}
