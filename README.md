# 42_minishell

WORK IN PROGRESS

Authors: Martijn Wallage, Julian Muller

## Project Description

Minishell is a project in the Core Curriculum of the 42 school. The aim is to recreate part of the Bash shell.

## Components

Our minishell (sometimes called "philoshell", since both authors are philosophers) consists of the following components:
- READER: takes input from the user, using the readline() function from the gnu readline library. It returns a char *.
- LEXER: a lexer could be a combination of a TOKENIZER, which splits the input into seperate words or tokens, and a - what shall we call it? - TYPER, which assigns a type to each token. So far, however, we found that we only really need a TOKENIZER. It returns a char **.
- PARSER: takes the char ** that it gets from the TOKENIZER and creates a linked list, with all the information necessary for the EXECUTOR to do its work. This includes:
	-- Splitting the line up into groups that form a simple command, separated by pipes |.
	-- Processing redirection: everything except the redirection itself. So the file is opened, but STDIN is not yet redirected to the fileno.
	-- If we do the bonus part, we will also need to handle parentheses and logical operators. Not sure yet.
- EXPANDER:
	-- expands variables
	-- removes quotes
	-- if we do the bonus part, we'll also have to handle wildcards.
- EXECUTOR
	-- Pipes.
	-- Redirects.
	-- Executes each simple command, either as an inbuilt function, a system command (alias) or a path to an executable.
- SIGNALS
	-- `ctrl-c`: interrupt child process and continue to next iteration of while loop. Sets exit code to 130.
	-- `ctrl-d` :interrupt all child processes and exit shell.
	-- `ctrl-/`: interrupt child process, if there is any. Otherwise do nothing.

## Current State

- All the basic functionality is there, except for SIGNALS and the bonus.
- We need better error handling.