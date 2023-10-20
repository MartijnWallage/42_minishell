# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/05 15:16:08 by mwallage          #+#    #+#              #
#    Updated: 2023/10/13 17:53:26 by mwallage         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			:= cc
CFLAGS		:= #-Wall -Wextra -Werror
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
				reader.c \
				lexer.c \
				parser.c \
				executor.c \
				pipex.c \
				utils.c \
				error.c \
				clean.c
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

$(NAME): $(LIBFT) $(OBJDIR) $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $< $(CFLAGS) $(HEADERS) -o $@

clean:
	rm -f $(OBJS);
	make clean -C$(LIBFTDIR)

fclean: clean
	rm $(NAME);
	rmdir $(OBJDIR);
	rm -rf $(LIBFTDIR);

re: fclean all

.PHONY: all clean fclean re
