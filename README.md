# 42_minishell

Authors: Martijn Wallage (mwallage), Julian Muller (jmuller)

## Project Description

Minishell is a project in the Core Curriculum of the 42 school. The aim is to recreate part of the Bash shell.
 
## Components

Our minishell (called "philoshell", since both authors are philosophers) consists of the following components:
- READER: takes input from the user, using the readline() function from the gnu readline library. It returns a char *.
- LEXER: a lexer could be a combination of a TOKENIZER, which splits the input into seperate words or tokens, and a - what shall we call it? - TYPER, which assigns a type to each token. We found, however, that we only really need a TOKENIZER. It returns a char **.
- PARSER: takes the char ** that it gets from the TOKENIZER and creates a linked list.
	- Splits the line up into groups that form a simple command, separated by pipes |.
	- handles parentheses as subshells.
- EXPANDER:
	- Removes redirect information from the command line (this information is by now parsed in our struct).
	- Expands wildcards.
	- Expands variables.
	- Removes quotes.
- EXECUTOR
	- Pipes and forks.
	- Redirects in four ways: 
		- `< infile`: redirect STD_IN to `infile`.
		- `<< END`: redirect STD_IN to a here_doc which ends with `END`. If `END` is without quotation marks, variables inside the heredoc get expanded.
		- `> outfile`: redirect STD_OUT to `outfile`, in truncate mode.
		- `>> outfile`: redirect STD_OUT to `outfile`, in append mode.
	- Executes each simple command, either as an inbuilt function, a system command (alias) or a path to an executable.
	- Runs subshells.

- SIGNALS
	- `ctrl-c`: interrupts child processes and continues to next iteration of while loop.
	- `ctrl-d`: end-of-file for here-doc and other child processes. If there are no childs, exits shell.
	- `ctrl-/`: stops child processes, if there are any; otherwise does nothing.
