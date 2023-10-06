# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mwallage <mwallage@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/05 15:16:08 by mwallage          #+#    #+#              #
#    Updated: 2023/10/05 17:13:46 by mwallage         ###   ########.fr        #
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
READLINE	:= -lreadline -lhistory
LIBFLAGS	:= -L$(LIBFTDIR) -lft $(READLINE)
SRC			:= main.c \
				pipex.c \
				pipex_utils.c
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
