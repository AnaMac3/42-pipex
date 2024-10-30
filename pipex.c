/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 11:05:02 by amacarul          #+#    #+#             */
/*   Updated: 2024/10/30 12:01:52 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpipex.h"

/*Execute a command by splitting its arguments and checking their validity.
Calls get_path to find the command path, and then executes the command*/
void	execute(char *cmd, char **envp, int is_first_cmd)
{
	char	*path;
	char	**cmd_args;

	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args || !cmd_args[0])
		handle_cmd_error(cmd, cmd_args, is_first_cmd);
	path = get_path(cmd, envp);
	if (!path || access(path, F_OK | X_OK) == -1)
	{
		if (path)
			free(path);
		handle_cmd_error(cmd, cmd_args, is_first_cmd);
		exit(EXIT_FAILURE);
	}
	if (execve(path, cmd_args, envp) == -1)
	{
		handle_error("cannot execute command", cmd_args[0]);
		free(path);
		free_array(cmd_args);
		exit (EXIT_FAILURE);
	}
}

//Set up and execute the first command in a pipeline.
void	child_process(char *file1, char *cmd1, int *pipefd, char **envp)
{
	int	fd;

	if (access(file1, F_OK) == -1 || access(file1, R_OK) == -1)
	{
		if (access(file1, F_OK) == -1)
			handle_error("no such file or directory", file1);
		else if (access(file1, R_OK) == -1)
			handle_error("permission denied", file1);
		close_pipefd(pipefd);
		exit (EXIT_FAILURE);
	}
	fd = open(file1, O_RDONLY);
	if (fd == -1)
	{
		handle_error("error opening file", file1);
		close_pipefd(pipefd);
		exit (EXIT_FAILURE);
	}
	close (pipefd[0]);
	dup2 (fd, 0);
	dup2 (pipefd[1], 1);
	close (fd);
	close (pipefd[1]);
	execute(cmd1, envp, 1);
}

//Set up and execute the last command in a pipeline.
void	parent_process(char *file2, char *cmd2, int *pipefd, char **envp)
{
	int	fd;

	waitpid(-1, NULL, 0);
	fd = open(file2, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1 || access(file2, W_OK) == -1)
	{
		if (fd == -1)
			handle_error("error opening file", file2);
		else if (access(file2, W_OK) == -1)
			handle_error("permission denied", file2);
		close_pipefd(pipefd);
		exit (EXIT_FAILURE);
	}
	close(pipefd[1]);
	dup2 (pipefd[0], 0);
	dup2 (fd, 1);
	close(pipefd[0]);
	close(fd);
	execute(cmd2, envp, 0);
}

//Main function
int	main(int argc, char **argv, char **envp)
{
	int		pipefd[2];
	pid_t	pid;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		exit (EXIT_FAILURE);
	}
	pid = do_pipe(pipefd);
	if (pid == 0)
		child_process(argv[1], argv[2], pipefd, envp);
	parent_process(argv[4], argv[3], pipefd, envp);
	return (0);
}
