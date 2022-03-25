# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ycornamu <marvin@42lausanne.ch>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/14 19:54:24 by ycornamu          #+#    #+#              #
#    Updated: 2022/03/25 17:35:19 by ycornamu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

SRCS = main.c \
	   philo.c \
	   philo_ext.c \
	   sim.c \
	   utils.c \
	   time.c \
	   cleanup.c \
	   exit.c
SRCS_DIR = src

OBJS = $(SRCS:.c=.o)
OBJS_DIR = obj

DIRS = obj

_SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS))
_OBJS = $(addprefix $(OBJS_DIR)/, $(OBJS))

HEADERS = includes

CC = gcc
CFLAGS = -I $(HEADERS)
LFLAGS = $(CFLAGS) -pthread

.PHONY : all debug clean fclean re

all: $(NAME)

debug: LFLAGS += -static-libasan
debug: CFLAGS += -g -fsanitize=address
debug: $(NAME)

$(NAME): $(_OBJS)
	$(CC) $(_OBJS) $(LFLAGS) -o $(NAME)

$(_OBJS): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c $(DIRS)
	$(CC) -c $(CFLAGS) $< -o $@

$(DIRS):
	mkdir -p $(DIRS)

clean:
	rm -rf $(OBJS_DIR)

fclean:
	rm -rf $(OBJS_DIR) $(NAME)

re: fclean all
