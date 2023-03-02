/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_exec_dir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakaddou <hakaddou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 03:59:03 by hakaddou          #+#    #+#             */
/*   Updated: 2023/02/04 15:04:58 by hakaddou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_directory(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}

int	file_exists(const char *pathname)
{
	struct stat	sb;

	return (stat(pathname, &sb) == 0);
}

int	dot_dir_check(t_cmd *cmd)
{
	if (!cmd->arg[0][0])
	{
		g_exit_code = COMMAND_FAIL;
		return (fd_printf(2, "minishell: : command not found\n"));
	}
	if (cmd->arg[0][0] == '.' && cmd->arg[0][1] == '\0')
	{
		fd_printf(2, "minishell: .: filename argument required\n.:");
		fd_printf(2, " usage: . filename [arguments]\n");
		g_exit_code = 2;
		return (1);
	}
	else if (cmd->arg[0][0] == '.' && cmd->arg[0][1] == '.'
			&& cmd->arg[0][2] == '\0')
	{
		g_exit_code = COMMAND_FAIL;
		return (fd_printf(2, "minishell: ..: command not found\n"));
	}
	else if (is_directory(cmd->arg[0]))
	{
		g_exit_code = 126;
		return (fd_printf(2, "minishell: %s: is a directory\n", cmd->arg[0]));
	}
	return (0);
}

int	check_exec_permission(char *cmd)
{
	int	i;

	i = 1;
	if (!cmd && !cmd[0] && !cmd[1] && !cmd[2] && cmd[0] != '.' && cmd[1] != '/')
		return (0);
	while (cmd[i] != '\0' && cmd[i] == '/')
		i++;
	if (access(cmd + i, F_OK) == 0)
	{
		if (access(cmd + i, X_OK) != 0)
		{
			fd_printf(2, "minishell: %s: permission denied\n", cmd);
			g_exit_code = 126;
			return (1);
		}
	}
	return (0);
}
