/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:09:24 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 17:55:14 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/**
 * goes through all pipes and closes both READ and WRITE end
*/
void	close_unused_pipes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->pipe_count)
	{
		close(data->pipes[i][READ]);
		close(data->pipes[i][WRITE]);
		i++;
	}
}

/**
 * does input redirection for the first and last command
 * and connects pipes to the correct ends
 * 
 * the STDIN of a process is always connected to previous pipe or infile
 * for 1st cmd
 * 
 * the STDOUT is always connected to the current pipe or outfile for last cmd
 * 
 * closes all pipe fds in the end after duping with close_unused_pipes
 * as they are no longer needed
*/
void	plumbing(t_data *data)
{
	if (data->cmd_count == 0)
	{
		if (dup2(data->infile, STDIN_FILENO) == -1)
			error_fatal("dup2 infile failed", data);
		close(data->infile);
	}
	if (data->cmd_count == (data->ncmds - 1))
	{
		if (dup2(data->outfile, STDOUT_FILENO) == -1)
			error_fatal("dup2 outfile failed", data);
		close(data->outfile);
	}
	if (data->cmd_count != 0)
	{
		if (dup2(data->pipes[data->cmd_count - 1][READ], STDIN_FILENO) == -1)
			error_fatal("dup2 READ end failed", data);
	}
	if (data->cmd_count != (data->ncmds - 1))
	{
		if (dup2(data->pipes[data->cmd_count][WRITE], STDOUT_FILENO) == -1)
			error_fatal("dup2 WRITE end failed", data);
	}
	close_unused_pipes(data);
}

/**
 * allocates memory for a 2d int array to safe all the pipe fds
 * nmcds-1 pipes are created
 * for every pipe created, the pipe_count is increased by one
*/
void	pipe_manufacturing(t_data *data)
{
	int	i;

	data->pipe_count = 0;
	data->pipes = malloc(sizeof(int *) * (data->ncmds - 1));
	if (!data->pipes)
		error_fatal("pipe malloc", data);
	i = 0;
	while (i < (data->ncmds - 1))
	{
		data->pipes[i] = malloc(sizeof(int) * 2);
		if (!data->pipes[i])
			error_fatal("pipe malloc", data);
		if (pipe(data->pipes[i]) == -1)
			error_fatal("pipe", data);
		data->pipe_count++;
		i++;
	}
}

/**
 * @param cmd command string to be executed
 * child is forked and executed command
 * 
 * in child:
 * plumbing connects all the pipes, does IO redirection, closes unused fds
 * execute_cmd executes execve and transforms the process to the program
 * process
 * in parent:
 * increases cmd_count by one and returns back to main function
*/
void	child_labor(char *cmd, t_data *data)
{
	pid_t		pid;

	pid = fork();
	if (pid == -1)
		error_fatal("fork", data);
	if (pid == 0)
	{
		plumbing(data);
		execute_cmd(cmd, data);
	}
	data->cmd_count++;
}
