# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwallage <mwallage@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/05 15:16:08 by mwallage          #+#    #+#              #
#    Updated: 2023/11/14 14:48:22 by mwallage         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			:= cc
# CFLAGS		:= -g -Wall -Wextra -Werror
SRCDIR		:= ./src
OBJDIR		:= ./obj
INCDIR		:= ./inc
LIBFTDIR	:= ./libft
LIBFT		:= $(LIBFTDIR)/libft.a
HEADERS		:= -I$(INCDIR) -I$(LIBFTDIR)/inc
READLINE_DIR := $(shell brew --prefix readline)
READLINE	:= -lreadline -lhistory -L $(READLINE_DIR)/lib
LIBFLAGS	:= -L$(LIBFTDIR) -lft $(READLINE)
SRC			:= main.c \
				reader/reader.c \
				lexer/lexer.c \
				lexer/tokenizer.c \
				lexer/lexer_utils.c \
				parser/parser.c \
				expander/expander.c \
				exec/executor.c \
				exec/exec.c \
				utils/utils.c \
				error/error.c \
				clean/clean.c \
				builtin/builtin.c \
				builtin/builtin_export.c \
				builtin/builtin_unset.c \
				builtin/builtin_cd.c
SRCS		:= $(addprefix $(SRCDIR)/, $(SRC))
OBJS		:= $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
NAME		:= minishell

all: $(NAME)

$(LIBFTDIR):
	git clone "https://github.com/MartijnWallage/42_libft.git" $(LIBFTDIR);

$(LIBFT): $(LIBFTDIR)
	make -C$(LIBFTDIR);
	
$(OBJDIR):
	mkdir obj;
	mkdir obj/reader;
	mkdir obj/lexer;
	mkdir obj/parser;
	mkdir obj/expander;
	mkdir obj/exec;
	mkdir obj/error;
	mkdir obj/clean;
	mkdir obj/utils;
	mkdir obj/builtin;

$(NAME): $(LIBFT) $(OBJDIR) $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CFLAGS) $(HEADERS) -o $@

clean:
	rm -rf $(OBJS);
	make clean -C$(LIBFTDIR)

fclean: clean
	rm -rf $(OBJDIR);
	rm -rf $(LIBFTDIR);
	rm $(NAME);

re: fclean all

.PHONY: all clean fclean re
