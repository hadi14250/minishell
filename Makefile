# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/05/06 19:58:12 by mmassarw          #+#    #+#              #
#    Updated: 2023/02/06 04:05:51 by hakaddou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	=	minishell

CC		=	@gcc
CFLAGS	=	-g3 -Wall -Wextra -Werror -I/usr/local/opt/readline/include# -fsanitize=address
LRLFLAG =	-L/usr/local/opt/readline/lib -lreadline

RM		=	@rm -f
ECHO	=	@echo

MAKELIB	=	@make -C libft
LIBFT	=	libft/libft.a
CLNLIB	=	@make clean -C libft
FCLNLIB	=	@make fclean -C libft

SRCS	=	SRCS/main.c \
			SRCS/execution/execution.c \
			SRCS/execution/builtins.c \
			SRCS/execution/ft_echo.c \
			SRCS/execution/ft_export.c \
			SRCS/execution/export_utils.c \
			SRCS/parsing/ft_parse_env.c \
			SRCS/parsing/ft_parse_token.c \
			SRCS/parsing/ft_parse_token_utils.c \
			SRCS/parsing/ft_tokenizer.c \
			SRCS/parsing/ft_collapsequotes.c \
			SRCS/parsing/ft_evaltokens.c \
			SRCS/parsing/ft_expandvar.c \
			SRCS/parsing/ft_tokenizer_utils.c \
			SRCS/parsing/ft_tokenlist.c \
			SRCS/utils/ft_free_list.c \
			SRCS/utils/temp_funs.c \
			SRCS/utils/ft_mani_list.c \
			SRCS/execution/ft_unset.c \
			SRCS/execution/ft_exit.c \
			SRCS/execution/env_conv.c \
			SRCS/execution/ft_cd.c \
			SRCS/execution/cd_utils.c \
			SRCS/execution/get_path_srcs.c \
			SRCS/execution/design.c \
			SRCS/execution/redirections.c \
			SRCS/execution/dups.c \
			SRCS/execution/redirections_srcs.c \
			SRCS/execution/check_exec_dir.c \
			SRCS/execution/execution_utils.c \
			SRCS/execution/heredoc.c \
			SRCS/execution/fd_handler.c \
			SRCS/execution/process_utils.c \
			SRCS/execution/pipe_utils.c \
			SRCS/execution/env_utils.c \
			SRCS/execution/redirections_srcs2.c \
			SRCS/utils/ft_signals.c \
			SRCS/execution/heredoc_utils.c \
			SRCS/execution/heredoc_utils2.c

OBJS	=	$(SRCS:.c=.o)

$(NAME):	$(OBJS)
			$(MAKELIB)
			$(ECHO) "Compiling minishell ......"
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LRLFLAG) 

all:		$(NAME)

clean:
			$(CLNLIB)
			$(RM) $(OBJS)

fclean:		
			$(FCLNLIB)
			$(ECHO) "Removing minishell ......."
			$(RM) $(NAME) $(OBJS)

re:			fclean all clean

exec:		all
			@./$(NAME)

rexec:		re
			@./$(NAME)

leak:
			make \
			&& clear \
			&& valgrind --leak-check=full \
			--suppressions=.rl.supp \
			--track-origins=yes \
			--show-leak-kinds=all -s \
			--track-fds=yes \
			./minishell

leaks:
			make re && make clean \
			&& clear \
			&& valgrind --leak-check=full \
			--suppressions=.rl.supp \
			--track-origins=yes \
			--show-leak-kinds=all -s \
			--track-fds=yes \
			./minishell

# --log-file="valg_errors.log"\
.PHONY: all clean fclean re