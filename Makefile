# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/19 09:17:44 by lbaumann          #+#    #+#              #
#    Updated: 2023/03/31 16:34:01 by lbaumann         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

CC = cc
CFLAGS = -Wall -Werror -Wextra

UNAME := $(shell uname)

ifeq ($(NOFLAGS), 1)
CFLAGS =
endif

ifeq ($(DEBUG), 1)
CFLAGS = -g
all: fclean
endif

SRCDIR = srcs/
INCDIR = includes/
OBJDIR = objs/
LIBFTDIR = libft/

# Colors
GR	= \033[32;1m
RE	= \033[31;1m
PU	= \033[35;1m
BL	= \033[34;1m
RC	= \033[0m

HEADERS = -I $(INCDIR)

SRCS = \
	srcs/main.c \
	srcs/command.c \
	srcs/pipex.c \
	srcs/utils.c \
	srcs/heredoc.c \

#libft
LIBFT = $(LIBFTDIR)libft.a
LIBFTINCL = -L$(LIBFTDIR) -lft

ifeq ($(UNAME), Linux)
endif
ifeq ($(UNAME), Darwin)
endif

SRC := $(notdir $(SRCS))
OBJ := $(SRC:.c=.o)
OBJS := $(addprefix $(OBJDIR), $(OBJ))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFTINCL) $(HEADERS) -o $(NAME)

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

$(LIBFT):
	if [ ! -d "libft" ]; then git clone https://github.com/leonyannick/libft.git; fi
	$(MAKE) -C $(LIBFTDIR)

clean:
	rm -rf $(OBJDIR)
	$(MAKE) clean -C $(LIBFTDIR)

cleanlibs:
	rm -rf $(LIBFTDIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFTDIR)

re: fclean all

.PHONY: all clean fclean re cleanlibs
