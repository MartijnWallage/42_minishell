# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/05 15:16:08 by mwallage          #+#    #+#              #
#    Updated: 2023/12/20 15:42:19 by mwallage         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror #-g -fsanitize=address
SRCDIR		:= ./src
OBJDIR		:= ./obj
INCDIR		:= ./inc
LIBFTDIR	:= ./libft
LIBFT		:= $(LIBFTDIR)/libft.a
HEADERS		:= -I$(INCDIR) -I$(LIBFTDIR)/inc
#READLINE_DIR := $(shell brew --prefix readline)
READLINE	:= -lreadline -lhistory -L $(READLINE_DIR)/lib
LIBFLAGS	:= -L$(LIBFTDIR) -lft $(READLINE)
HOST		:= $(shell hostname)
SRC			:= main.c \
				reader/reader.c \
				tokenizer/tokenizer.c \
				parser/parse_heredoc.c \
				parser/parser.c \
				parser/parser_utils.c \
				parser/syntax_check.c \
				parser/syntax_utils.c \
				expander/expand_heredoc.c \
				expander/expander.c \
				expander/expander_utils.c \
				expander/expand_wildcards.c \
				expander/expander_remove.c \
				exec/exec_heredoc.c \
				exec/exec_pipe.c \
				exec/exec_redirect.c \
				exec/exec_utils.c \
				exec/executor.c \
				signals/signals.c \
				utils/utils.c \
				utils/utils_checks.c \
				utils/utils_tab.c \
				clean/error.c \
				clean/clean.c \
				builtin/builtin.c \
				builtin/builtin_export.c \
				builtin/builtin_unset.c \
				builtin/builtin_cd.c
SRCS		:= $(addprefix $(SRCDIR)/, $(SRC))
OBJS		:= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
NAME		:= minishell

all: $(NAME)

$(LIBFT): $(LIBFTDIR)
	make -C$(LIBFTDIR);
	
$(OBJDIR):
	mkdir obj;
	mkdir obj/reader;
	mkdir obj/tokenizer;
	mkdir obj/parser;
	mkdir obj/expander;
	mkdir obj/exec;
	mkdir obj/clean;
	mkdir obj/utils;
	mkdir obj/builtin;
	mkdir obj/signals;

$(NAME): $(LIBFT) $(OBJDIR) $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -DHOSTNAME=\"$(HOST)\" -c $< $(CFLAGS) $(HEADERS) -o $@

clean:
	rm -rf $(OBJS);
	make clean -C$(LIBFTDIR)

fclean: clean
	rm -rf $(OBJDIR);
	make fclean -C$(LIBFTDIR);
	rm $(NAME);

re: fclean all

.PHONY: all clean fclean re
