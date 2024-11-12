/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:29:37 by amacarul          #+#    #+#             */
/*   Updated: 2024/11/12 15:19:13 by amacarul         ###   ########.fr       */
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

/*Set up and execute a here_doc*/
/*void	here_doc(char **argv, int *prev_pipefd)
{
	pid_t	pid;
	char	*line;

	pid = do_pipe(prev_pipefd);
	if (pid == 0)
	{
		close(prev_pipefd[0]);
		line = get_next_line(0);
		while (line && ft_strncmp(line, argv[2], ft_strlen(argv[2])) != 0)
		{
			if (*line != '\0')
			{
				ft_putstr_fd(line, prev_pipefd[1]);
				if (line[ft_strlen(line) - 1] != '\n')
					ft_putchar_fd('\n', prev_pipefd[1]);
			}
			free(line);
			line = get_next_line(0);
		}
		free(line);
		close(prev_pipefd[1]);
		exit (0);
	}
	close(prev_pipefd[1]);
	dup2(prev_pipefd[0], 0);
	close(prev_pipefd[0]);
	waitpid(-1, NULL, 0);
}*/

/*Set up and execute the first command in a pipeline.*/
void	first_process(char *file1, char *cmd1, int *pipefd, char **envp)
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

/*Set up and execute the last command in a pipeline.*/
void	last_process(int argc, char **argv, int *pipefd, char **envp)
{
	int	fd;
	char	*cmd;
	
	cmd = argv[argc - 2];
	waitpid(-1, NULL, 0);
	/*if (ft_strcmp(argv[1], "here_doc") == 0)
		fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_APPEND, 0644);
	else*/
		fd = open(argv[argc - 1], O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1 || access(argv[argc - 1], W_OK) == -1)
	{
		if (fd == -1)
			handle_error("error opening file", argv[argc - 1]);
		else if (access(argv[argc - 1], W_OK) == -1)
			handle_error("permission denied", argv[argc - 1]);
		close_pipefd(pipefd);
		exit (EXIT_FAILURE);
	}
	close(pipefd[1]);
	dup2 (pipefd[0], 0);
	dup2 (fd, 1);
	close(pipefd[0]);
	close(fd);
	execute(cmd, envp, 0);
}

/*Set up and execute an intermediate command in a pipeline.
	- waitpid(): waits for any child process to finish
	- closes fds it the read end of the current pipe (pipefd[0])
	and the write end of previous pipe (prev_pipefd[1]) 
	as there are not used here
	- dup2() to redirect stdin to prev_pipefd[0] (read end of previous pipe)
	- dup2() to redirect stdout to pipefd[1] (write end of current pipe).
	This redirects the cmds output to the next process in the pipeline
	- close the original fds after redirection
	- call execute() to run the cmd. The cmd reads from the redirected
	stdin (prev_pipefd[0]) and writes its output to the redirected stdout
	(pipefd[1])*/
void	inter_process(char *cmd, int *prev_pipefd, int *pipefd, char **envp)
{
	waitpid(-1, NULL, 0);
	close(pipefd[0]);
	close(prev_pipefd[1]);
	dup2(prev_pipefd[0], 0);
	dup2(pipefd[1], 1);
	close(prev_pipefd[0]);
	close(pipefd[1]);
	execute(cmd, envp, 0);
}
