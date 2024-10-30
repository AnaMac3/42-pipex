/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:17:43 by amacarul          #+#    #+#             */
/*   Updated: 2024/10/30 11:20:10 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpipex.h"

//Print an error message to stderror
void	handle_error(char *msg, char *name)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(name, 2);
	ft_putchar_fd('\n', 2);
}

/*Handle command error, print the message and free cmd_args.
If it is the first command, it closes stdout and exits*/
void	handle_cmd_error(char *cmd, char **cmd_args, int is_first_cmd)
{
	handle_error("command not found", cmd);
	free_array(cmd_args);
	if (is_first_cmd)
	{
		close(1);
		exit (EXIT_FAILURE);
	}
}

//Create a pipe and fork a child process
pid_t	do_pipe(int pipefd[2])
{
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("Pipe");
		exit (EXIT_FAILURE);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("Fork");
		exit (EXIT_FAILURE);
	}
	return (pid);
}

//Free an array of strings
void	free_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free (array[i]);
		i ++;
	}
	free(array);
}

//Close both ends of a pipeline
void	close_pipefd(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}
