# 42_minishell

WORK IN PROGRESS

Authors: Martijn Wallage (mwallage), Julian Muller (jmuller)

## Project Description

Minishell is a project in the Core Curriculum of the 42 school. The aim is to recreate part of the Bash shell.
 
## Components

Our minishell (called "philoshell", since both authors are philosophers) consists of the following components:
- READER: takes input from the user, using the readline() function from the gnu readline library. It returns a char *.
- LEXER: a lexer could be a combination of a TOKENIZER, which splits the input into seperate words or tokens, and a - what shall we call it? - TYPER, which assigns a type to each token. So far, however, we found that we only really need a TOKENIZER. It returns a char **.
- PARSER: takes the char ** that it gets from the TOKENIZER and creates a linked list, with all the information necessary for the EXECUTOR to do its work.
	- Splits the line up into groups that form a simple command, separated by pipes |.
	- Pre-processes redirection: everything except the redirection itself. So the file is OPEN_SUBSHELLed, but STDIN is not yet redirected to the fileno.
	- If we do the bonus part, we will also need to handle parentheses and logical operators. Not sure yet if we will.
- EXPANDER:
	- expands variables
	- removes quotes
	- if we do the bonus part, we'll also have to handle wildcards.
- EXECUTOR
	- Pipes and forks.
	- Redirects in four ways: 
		-`< infile`: redirect STD_IN to `infile`.
		-`<< END`: redirect STD_IN to a here_doc which ends with `END`.
		-`> outfile`: redirect STD_OUT to `outfile`, in truncate mode.
		-`>> outfile`: redirect STD_OUT to `outfile`, in append mode.
	- Executes each simple command, either as an inbuilt function, a system command (alias) or a path to an executable.

- SIGNALS
	- `ctrl-c`: interrupt child process and continue to next iteration of while loop. Sets exit code to 130.
	- `ctrl-d`: interrupt all child processes and exit shell.
	- `ctrl-/`: interrupt child process, if there is any. Otherwise do nothing.

## Current State

- All the basic functionality is there, except for SIGNALS.
- There are some memory leaks to fix.
- wildcards with multiple results only displaying one result
