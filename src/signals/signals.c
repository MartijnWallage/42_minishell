

#include "minishell.h"

// 1. Goal: Make sure the main process keeps running

volatile	sig_atomic_t sig_flag = 0;

// chose close resemblence with bash?
void	handle_sigint(int sig)
{
	(void) sig;

	write(1, "\n", 2);
	rl_on_new_line();
	rl_redisplay();
	rl_replace_line("  ", 1);
	sig_flag = 1;
}

void	handle_sigquit(int sig)
{
	(void) sig;

	write(1, "\n", 2);
	rl_on_new_line();
	rl_redisplay();
	rl_replace_line("  ", 1);
	sig_flag = 1;
}


void	signal_handler()
{
	struct sigaction act;

	act.sa_handler = handle_sigint;
	act.sa_flags = 0; // or SA_RESTART to restart system calls
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);

	act.sa_handler = handle_sigquit;
	sigaction(SIGQUIT, &act, NULL);
}
