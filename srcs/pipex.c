/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:09:24 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 12:10:10 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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

void	plumbing(t_data *data)
{
	if (data->cmd_count == 0)
	{
		dup2(data->infile, STDIN_FILENO);
		close(data->infile);
	}
	if (data->cmd_count == (data->ncmds - 1))
	{
		dup2(data->outfile, STDOUT_FILENO);
		close(data->outfile);
	}
	if (data->cmd_count != 0)
		dup2(data->pipes[data->cmd_count - 1][READ], STDIN_FILENO);
	if (data->cmd_count != (data->ncmds - 1))
		dup2(data->pipes[data->cmd_count][WRITE], STDOUT_FILENO);
	close_unused_pipes(data);
}

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
