#include "../inc/minishell.h"

static int	is_valid_input(char *input)
{
	if (*input == '?') // just to test for now
	{
		ft_printf("%s\n", INVALID_INPUT);
		return (0);
	}
	return (1);
}

char	*reader(void)
{
	char	*str;
	
	str = readline(PROMPT);
	if (!str)
	{
		printf("No line\n");
		return (NULL);
	}
	if (!is_valid_input(str))
	{
		free(str);
		return (NULL);
	}
	return (str);
}
