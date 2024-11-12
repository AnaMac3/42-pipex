/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:05:29 by amacarul          #+#    #+#             */
/*   Updated: 2024/11/12 15:18:53 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpipex.h"

void	first_process_infile(char **argv, int *prev_pipefd, char **envp)
{
	pid_t	pid;

	pid = do_pipe(prev_pipefd);
	if (pid == 0)
		first_process(argv[1], argv[2], prev_pipefd, envp);
	close(prev_pipefd[1]);
}

void	process_cmds(int argc, char **argv, int *prev_pipefd, char **envp)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;

	i = 2;
	while (i <= argc - 3)
	{
		pid = do_pipe(pipefd);
		if (pid == 0)
		{
			if (i == argc - 3)
				last_process(argc, argv, prev_pipefd, envp);
			else
				inter_process(argv[i + 1], prev_pipefd, pipefd, envp);
		}
		close(prev_pipefd[0]);
		close(pipefd[1]);
		prev_pipefd[0] = pipefd[0];
		prev_pipefd[1] = pipefd[1];
		i ++;
	}
	waitpid(-1, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		pipefd[2];

	if (argc < 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\n", 2);
		ft_putstr_fd("	./pipex here_doc cmd cmd1 file\n", 2);
		exit (EXIT_FAILURE);
	}
	first_process_infile(argv, pipefd, envp);
	process_cmds(argc, argv, pipefd, envp);
	waitpid(-1, NULL, 0);
	return (0);
}
