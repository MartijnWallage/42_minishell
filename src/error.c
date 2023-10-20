#include "../inc/minishell.h"

void	handle_error(char *info, int exitcode)
{
	char	*errno_readable;
	int		len;

	errno_readable = strerror(errno);
	len = ft_strlen(errno_readable);
	write(2, "philoshell: ", 7);
	write(2, errno_readable, len);
	len = ft_strlen(info);
	write(2, ": ", 2);
	write(2, info, len);
	write(2, "\n", 1);
	exit(exitcode);			// not allowed!
}