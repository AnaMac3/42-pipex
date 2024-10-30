/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:09:12 by amacarul          #+#    #+#             */
/*   Updated: 2024/10/30 12:04:39 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpipex.h"

//Get the path variable from the enviromental variable array
char	*get_path_from_envp(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp (envp[i], "PATH=", 5) == 0)
		{
			path = ft_substr(envp[i], 5, (ft_strlen(envp[i]) - 5));
			return (path);
		}
		i ++;
	}
	return (NULL);
}

/*Build the command path by searching through the provided directories.
Return the full path if the command is found and NULL if not.*/
char	*build_cmd_path(char **path_dirs, char **cmd_args)
{
	char	*cmd_path;
	char	*exec;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		cmd_path = ft_strjoin(path_dirs[i], "/");
		if (!cmd_path)
			return (NULL);
		exec = ft_strjoin(cmd_path, cmd_args[0]);
		free(cmd_path);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_array(path_dirs);
			free_array(cmd_args);
			return (exec);
		}
		free(exec);
		i ++;
	}
	return (NULL);
}

/*Get the executable path for a given command.
If the command is not found, returns NULL or the command itself if
not in PATH*/
char	*get_path(char *cmd, char **envp)
{
	char	*path_var;
	char	**path_dirs;
	char	**cmd_args;
	char	*exec;

	path_var = get_path_from_envp(envp);
	if (!path_var)
		return (NULL);
	path_dirs = ft_split(path_var, ':');
	cmd_args = ft_split(cmd, ' ');
	free(path_var);
	if (!path_dirs || !cmd_args)
	{
		free_array(cmd_args);
		free_array(path_dirs);
		return (NULL);
	}
	exec = build_cmd_path(path_dirs, cmd_args);
	if (exec)
		return (exec);
	free_array(cmd_args);
	free_array(path_dirs);
	return (ft_strdup(cmd));
}
