/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 19:26:15 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/06 01:23:01 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

# include <stdbool.h>
# include <curses.h>
# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <string.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stddef.h>

//	colors
# define BLUE_FONT	"\033[1;36m"
# define RESET_FONT	"\033[0m"
# define RED_FONT	"\033[31m"
# define RF	"\033[0m"
// eeror strings
# define ALPHA_EXIT "minishell: exit: %s: numeric argument required\n"
# define UNSET_NO_ARG "unset: not enough arguments\n"

# define WLC "\033[1;37m\nThe default interactive shell is now minishell.\nIf"
# define WLC2 " you like this shell, please give us a follow on\n"
# define WLC3 "\033[1;4;34mgithub.com/hadi14250\033[0m\033[1;37m &\033[0m "
# define WLC4 "\033[1;4;34mgithub.com/mmassarw\033[0m\033[1;37m \n\nAbout"
# define WLC5 " your shell 👇\n • 🐚 shell level: %d\n • 👤 User: %s\n\n\033[0m"

// command flags
# define BUILTIN 68
# define PWD 923
# define VALUE 432
# define KEY 645

//builtin exit codes
# define SUCCESS 0
# define PWD_FAIL_CODE 2
# define ENV_FAIL_CODE 1
# define EXPORT_FAIL_CODE 1
# define EXPORT_FLAG 654
# define UNSET_FLAG 456
# define UNSET_FAIL_CODE 1
# define COMMAND_FAIL 127
# define EXIT_FAIL 1
# define EXIT_ALPHA_CODE 255
# define CD_FAIL 1
# define EXIST 0

// color pallete
# define BLUE 1
# define WHITE 2
# define YELLOW 3
# define GREEN 4
# define PURPLE 5
# define DARK_CYAN 6

// redirections enumiration
enum e_rdr
{
	NONE,
	INPUT,
	OUTPUT,
	HEREDOC,
	APPEND,
};

// redirections linked list
typedef struct s_rdr
{
	char			*file;
	int				fd;
	int				og_fd;
	int				dup2_fd;
	pid_t			fork_id;
	int				ret;
	int				fdpipe[2];
	int				herepipe[2];
	enum e_rdr		e_rdr;
	struct s_rdr	*next;
}	t_rdr;

// token enumiration
typedef enum e_tokentype
{
	SPACES,
	SINGLE,
	DOUBLE,
	VARIABLE,
	REDIRECTION,
	PIPE,
	WORD,
}	t_type;

// token structure
typedef struct s_token
{
	char				*content;
	enum e_tokentype	type;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

// command linked list
typedef struct s_cmd
{
	char			**arg;
	t_rdr			*rdr;
	int				o_fd;
	int				i_fd;
	int				odup2_fd;
	int				idup2_fd;
	int				fd_pipe[2];
	struct s_cmd	*next;
	pid_t			fork_id;
}	t_cmd;

// enviromental linked list
typedef struct s_env
{
	char			*key;
	char			*value;
	bool			initialised;
	struct s_env	*next;
}	t_env;

// struct with all the other structs in it
typedef struct s_mini
{
	t_cmd	*l_cmd;
	t_env	*l_env;
	char	*rl;
	t_token	*l_token;
	int		cmd_flag;
}	t_mini;

typedef struct s_hrdoc
{
	char	*i;
	char	*total;
	char	*tmp;
	char	*line;
	bool	quoted;
}	t_hrdoc;

// global exit code
int	g_exit_code;

// temporary functions, tbc if they'd be used or not

int		random_between(int min, int max);

// builtins

void	print_pwd(void);
void	ft_echo(char **args);
void	ft_export(char **args, t_mini *mini);
void	ft_unset(char **args, t_mini *mini);
void	ft_exit(char **args, t_mini *mini);
int		builtin_check(t_mini *mini, t_cmd *cmd);

// export

void	ft_export(char **args, t_mini *mini);
void	parse_new_export(char *arg, t_mini *mini);
void	ft_modify_env(char *arg, t_mini *mini);
t_env	*env_already_exist(char *arg, t_mini *mini);
int		check_export_args(char *arg);
int		check_valid_identifier(char *arg);
void	add_to_env(char *arg, t_mini *mini);
char	*set_env_value(char *arg, t_env *new);
char	*set_env_key(char *arg);
void	print_export(t_mini *mini);

// tokenizing

void	ft_tokenize(t_mini *mini);
char	*ft_check_var(char *quote, t_mini *mini);
t_type	ft_label_token(char c);
void	ft_eovchr(size_t *i, const char *str, enum e_tokentype *type);
bool	ft_syntaxerr(char *errmsg, int num, bool ret);
void	ft_tokenlist(t_mini *mini);
bool	ft_evaltokens(t_mini *mini);
void	ft_collapsequotes(t_mini *mini);
void	ft_expandvar(t_mini *mini);

// parsing

int		ft_check_rdr(char *string);
void	ft_populate_cmd(t_mini *mini, t_cmd *cmd, t_token **token);
int		ft_count_till_pipe(t_token *token);
void	ft_parse_env(t_mini *mini, const char **envp);
void	ft_parse_token(t_mini *mini);
void	set_env_underscore(char *cmd, t_mini *mini);

// frees

void	ft_free_cmd(t_cmd *s_cmd);
void	ft_free_ltoken(t_token *list);
void	ft_free_env(t_env *env_list);
void	ft_free_cycle(t_mini *mini);
void	ft_free_all(t_mini *mini);

// exit shell

void	ft_exit_shell(t_mini *m, int er, char *p_er, int fd);

// printing

// void	ft_print_cmd(t_cmd *s_head);
// void	print_env(t_mini *mini);
// void	ft_print_split(char **split);
// void	print_linked_list_by_type(t_token *head);

// utils

void	parse_input(t_mini *mini);
bool	ft_syntaxerr(char *errmsg, int num, bool ret);
void	pop_node(t_token **list, t_token *node);
void	add_node_middle(t_token *node_before, t_token *new_node);
void	delete_empty_nodes(t_token **head);
void	join_same_type_nodes(t_token *head);

// unset

void	ft_unset(char **args, t_mini *mini);
int		check_valid_identifier_export(char *arg);
int		check_unset_args(char *arg);
void	free_single_env(t_env *node);
void	delete_env_list(char *arg, t_mini *mini);

// exit

void	ft_exit(char **args, t_mini *mini);
int		arg_count(char **args);
int		check_exit_alpha(char **args);
void	exit_and_print(int code);
void	exit_success(char **args, t_mini *mini);

// chdir (cd)

void	ft_cd(char **args, t_mini *mini);
void	go_to_home(t_mini *mini, char *old_pwd);
void	go_to_old_pwd(t_mini *mini, char *old_pwd);
void	set_env_pwd(t_mini *mini);
void	set_old_pwd(t_mini *mini, char *old_pwd);
void	cd_return_success(t_mini *mini, char *old_pwd);
char	*find_str_env(char *arg, t_mini *mini, int flag);

// env **char conversion from linked list

void	*perror_return(char *str, void *ret);
int		return_env_size(t_env *env);
char	*join_key_val(char *key, char *value);
char	*join_key_eq(char *key, t_env *env);
char	**convert_env(t_mini *mini);
void	print_env(t_mini *mini);

// get path srcs

char	*split_and_join(char *av_cmd, char *splitted);
int		word_count(char const *s, char c);
char	*get_path(char *cmd, char *env);

// design and colors functions

char	*read_line_colored(int random);
char	*read_line_colored2(int random, char *path);

// redirections

int		error_set_print_close(t_mini *mini, t_cmd *cmd, int error);
int		ft_dup2_output(t_rdr *rdr);
int		ft_dup2_input(t_rdr *rdr);
int		ft_dup2_append(t_rdr *rdr);
int		parse_dups(t_rdr *trdr, t_mini *mini, t_cmd *cmd);
int		parse_redirect(t_mini *mini, t_cmd *cmd);
int		file_no_exist(t_mini *mini, t_rdr *trdr);
int		ft_return_redirect_code(t_rdr *rdr);
int		check_file_rights(t_mini *mini, t_rdr *trdr);
int		ft_redirect(t_mini *mini, t_cmd *cmd);
void	ft_close_rdr_backv2(t_rdr *ordr, t_rdr *irdr);
void	close_rdr_back(t_cmd *cmd);
bool	empty_file_check(t_rdr *rdr);
int		parse_redirect_errors(t_mini *mini, t_cmd *cmd, int flag);
int		error_set_print_close(t_mini *mini, t_cmd *cmd, int error);
void	close_rdr_files(t_mini *mini, t_cmd *cmd);

// directory check and direct execution (ex: ./minishell)

int		is_directory(const char *path);
int		file_exists(const char *pathname);
int		dot_dir_check(t_cmd *cmd);

// heredoc

bool	parse_delimiter(t_rdr *rdr);
void	trim_last_new_line(char *file);
void	send_input_to_parent(t_mini *mini, char *i, t_rdr *r, char *t);
void	take_heredoc_input(t_rdr *rdr, t_mini *mini);
int		ft_pipe_heredoc(t_rdr *rdr, t_mini *mini, t_cmd *cmd);
void	read_heredoc_child(t_rdr *rdr);
int		ft_fork_heredoc(t_rdr *rdr);
int		pipe_heredoc(t_rdr *rdr);
int		take_input_from_child(t_mini *mini, t_rdr *rdr);
int		handle_heredoc(t_mini *mini);

// fd handlers

void	close_all_fds(t_mini *mini);
int		ft_close(int fd, int limit, t_cmd *cmd);

// children and process unstifle_historyint

int		is_parent_compatible(t_cmd *cmd);
int		is_parent_exec(t_cmd *cmd);
void	execute_in_parent(t_mini *mini);
void	wait_for_children(t_mini *mini);

// piping and execution

int		is_slash_exec(t_mini *mini, t_cmd *cmd);
int		check_exec_permission(char *cmd);
void	execute_in_dir(t_mini *mini, t_cmd *cmd);
void	execute_command_fork(t_mini *mini, t_cmd *cmd, char *cmd_path);
void	execute_pathed_cmd(t_mini *mini, t_cmd *cmd);
void	execute_in_child(t_mini *mini);
void	exec_children_cmds(t_mini *mini, t_cmd *cmd);
int		ft_fork(t_cmd *cmd, t_mini *mini);
void	dup_pipe_output(t_mini *mini, t_cmd *cmd);
void	dup_pipe_input(t_mini *mini, t_cmd *cmd, t_cmd *prevcmd);
void	close_pipe(t_cmd *prevcmd);
int		ft_pipe(int fdpipe[2], t_mini *mini);

// env utils

void	set_env_underscore(char *cmd, t_mini *mini);
void	add_shlvl(t_env *env, t_mini *mini);
void	increase_shlvl(t_env *env, t_mini *mini);
char	**add_basic_env(t_mini *mini);

// signals

void	ft_interupt(int sig);
void	ft_intheredoc(int sig);

#endif
