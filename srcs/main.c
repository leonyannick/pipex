/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 11:24:33 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_int_arr(int **ptr, int nrows)
{
	int	i;

	i = 0;
	while (i < nrows)
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}

void	error_fatal(char *e_msg, t_data *data)
{
	if (!data)
		free_int_arr(data->pipes, data->pipe_count);
	perror(e_msg);
	exit(errno);
}

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


void	child_labor(char *cmd, t_data *data)
{
	pid_t		pid;

	pid = fork();
	if (pid == -1)
		error_fatal("fork", data);
	if (pid == 0)
	{
		plumbing(data);
		execute_cmd(cmd);
	}
}

void	usage(void)
{
	ft_printf("Note: not enough parameters\n");
	ft_printf("./pipex <infile> <cmd1> <cmd2> ... <cmdn> <outfile>\n");
	ft_printf("./pipex here_doc <LIMITER> <cmd> <cmd1> <file>\n");
	exit(EXIT_SUCCESS);
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

int	main(int argc, char **argv)
{
	int		i;
	t_data	data;

	data.ncmds = argc - 3;
	data.cmd_count = 0;
	data.infile = open(argv[1], O_RDONLY);
	data.outfile = open(argv[argc - 1], O_RDWR);
	if (data.outfile == -1 || data.infile == -1)
		error_fatal("infile/outfile", NULL);
	if (argc < 5)
		usage();
	pipe_manufacturing(&data);
	i = 2;
	while (i < argc - 1)
	{
		child_labor(argv[i], &data);
		data.cmd_count++;
		i++;
	}
	i = 2;
	close(data.infile);
	close(data.outfile);
	close_unused_pipes(&data);
	free_int_arr(data.pipes, data.pipe_count);
	while (i < argc - 1)
	{
		wait(NULL);
		i++;
	}
	return (EXIT_SUCCESS);
}
