#include <stdlib.h>

int	main(void)
{
	char	*str;

	*str = 'a';
	free(str);
	return (0);
}

