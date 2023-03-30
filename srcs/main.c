/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/30 18:05:37 by lbaumann         ###   ########.fr       */
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
		
		i++;
	}
}

void	plumbing(t_data *data, e_redirect redirect)
{
	if (redirect == INPUT)
	{
		dup2(data->infile, STDIN_FILENO);
		close(data->infile);
		close(data->pipes[data->cmd_count][READ]);
		dup2(data->pipes[data->cmd_count][WRITE], STDOUT_FILENO);
		close(data->pipes[data->cmd_count][WRITE]);
	}
	else if (redirect == OUTPUT)
	{
		close(data->pipes[data->cmd_count - 1][WRITE]);
		dup2(data->pipes[data->cmd_count - 1][READ], STDIN_FILENO);
		close(data->pipes[data->cmd_count - 1][READ]);
		dup2(data->outfile, STDOUT_FILENO);
		close(STDOUT_FILENO);
	}
	else
	{
		//read end
		close(data->pipes[data->cmd_count - 1][WRITE]);
		dup2(data->pipes[data->cmd_count - 1][READ], STDIN_FILENO);
		close(data->pipes[data->cmd_count - 1][READ]);
		
		//write end
		close(data->pipes[data->cmd_count][READ]);
		dup2(data->pipes[data->cmd_count][WRITE], STDOUT_FILENO);
		close(data->pipes[data->cmd_count][WRITE]);
	}
	close_unused_pipes(data);
}


void	child_labor(char *cmd, t_data *data)
{
	pid_t		pid;
	e_redirect	redirect;

	redirect = PIPE;
	pid = fork();
	if (pid == -1)
		error_fatal("fork", data);
	if (pid == 0)
	{
		if (data->cmd_count == 0)
			redirect = INPUT;
		else if (data->cmd_count == (data->ncmds - 1))
			redirect = OUTPUT;
		plumbing(data, redirect);
		// close(p[0]);
		// dup2(p[1], STDOUT_FILENO);
		// close(p[1]);
		execute_cmd(cmd);
	}
	return (1);
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
	// if (argc < 5)
	// 	usage();
	pipe_manufacturing(&data);

	// i = 0;
	// while (i < ncmds)
	// {
	// 	child_labor(argv[i], pipes);
	// 	i++;
	// }
	// while (i < argc - 1)
	// {
	// 	wait(NULL);
	// 	i++;
	// }
	
	
	// infile = open(argv[1], O_RDONLY);
	// outfile = open(argv[argc - 1], O_RDWR);
	// if (outfile == -1 || infile == -1)
	// 	error_fatal("infile/outfile");
	// dup2(infile, STDIN_FILENO);
	// close(infile);
	// close(outfile);
	// i = 2;
	// while (i < argc - 1)
	// {
	// 	if (pipe(p) == -1)
	// 		error_fatal("pipe");
	// 	if (i == (argc - 2))
	// 	{
	// 		dup2(outfile, STDOUT_FILENO);
	// 		close(STDOUT_FILENO);
	// 	}
	// 	child_labor(argv[i], p);
	// 	close(p[1]);
	// 	dup2(p[0], STDIN_FILENO);
	// 	close(p[0]);
	// 	i++;
	// }
	// i = 2;
	// while (i < argc - 1)
	// {
	// 	wait(NULL);
	// 	i++;
	// }
	
	return (EXIT_SUCCESS);
}
