/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libpipex.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:10:47 by amacarul          #+#    #+#             */
/*   Updated: 2024/10/30 10:57:50 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBPIPEX_H
# define LIBPIPEX_H
# include "my_libft/libft.h"
# include <unistd.h> //fork, access
# include <fcntl.h> //pid_t
# include <sys/wait.h> //waitpid
# include <stdio.h> //perror, strerror

//pipex.c
void	execute(char *cmd, char **envp, int is_first_cmd);
void	child_process(char *file1, char *cmd1, int *pipefd, char **envp);
void	parent_process(char *file2, char *cmd2, int *pipefd, char **envp);
int		main(int argc, char **argv, char **envp);

//pipex_utils.c
void	handle_error(char *msg, char *name);
void	handle_cmd_error(char *cmd, char **cmd_args, int is_first_cmd);
pid_t	do_pipe(int pipefd[2]);
void	free_array(char **array);
void	close_pipefd(int *pipefd);

//get_path.c
char	*get_path_from_envp(char **envp);
char	*build_cmd_path(char **path_dirs, char **cmd_args);
char	*get_path(char *cmd, char **envp);

//BONUS
//pipex_bonus.c
void	execute(char *cmd, char **envp, int is_first_cmd);
void	here_doc(char **argv, int *prev_pipefd);
void	first_process(char *file1, char *cmd1, int *pipefd, char **envp);
void	inter_process(char *cmd, int *prev_pipefd, int *pipefd, char **envp);
void	last_process(char *file2, char *cmd2, int *pipefd, char **envp);

//main_bonus.c
void	first_process_infile(char **argv, int *prev_pipefd, char **envp);
void	process_cmds(int argc, char **argv, int *prev_pipefd, char **envp);
int		main(int argc, char **argv, char **envp);

#endif