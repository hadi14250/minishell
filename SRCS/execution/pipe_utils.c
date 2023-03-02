/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/03 20:20:42 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/03 20:25:54 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_fork(t_cmd *cmd, t_mini *mini)
{
	cmd->fork_id = fork();
	if (cmd->fork_id == -1)
	{
		fd_printf(2, "minishell: fork: %s\n", strerror(errno));
		g_exit_code = 1;
		wait_for_children(mini);
		return (-1);
	}
	return (cmd->fork_id);
}

void	dup_pipe_output(t_mini *mini, t_cmd *cmd)
{
	cmd->fd_pipe[0] = ft_close(cmd->fd_pipe[0], 3, NULL);
	cmd->odup2_fd = dup2(cmd->fd_pipe[1], STDOUT_FILENO);
	if (cmd->odup2_fd == -1)
		ft_exit_shell(mini, errno, strerror(errno), 2);
	cmd->fd_pipe[1] = ft_close(cmd->fd_pipe[1], 0, NULL);
}

void	dup_pipe_input(t_mini *mini, t_cmd *cmd, t_cmd *prevcmd)
{
	prevcmd->fd_pipe[1] = ft_close(prevcmd->fd_pipe[1], 3, cmd);
	cmd->idup2_fd = dup2(prevcmd->fd_pipe[0], STDIN_FILENO);
	if (cmd->idup2_fd == -1)
		ft_exit_shell(mini, errno, strerror(errno), 2);
	cmd->fd_pipe[0] = ft_close(cmd->fd_pipe[0], 0, cmd);
}

void	close_pipe(t_cmd *prevcmd)
{
	prevcmd->fd_pipe[0] = ft_close (prevcmd->fd_pipe[0], 0, NULL);
	prevcmd->fd_pipe[1] = ft_close (prevcmd->fd_pipe[1], 0, NULL);
}

int	ft_pipe(int fdpipe[2], t_mini *mini)
{
	int	error;

	error = pipe(fdpipe);
	if (error == -1)
	{
		g_exit_code = errno;
		fd_printf(2, "minishell: pipe: %s\n", strerror(errno));
		wait_for_children(mini);
		return (-1);
	}
	else
		return (0);
}
