/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/04/04 10:19:24 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	usage(void)
{
	ft_printf("Note: not enough parameters\n");
	ft_printf("./pipex <infile> <cmd1> <cmd2> ... <cmdn> <outfile>\n");
	ft_printf("./pipex here_doc <LIMITER> <cmd> <cmd1> <file>\n");
	exit(EXIT_SUCCESS);
}

void	init_data(char **argv, int argc, t_data *data)
{
	data->ncmds = argc - 3;
	data->cmd_count = 0;
	data->infile = open(argv[1], O_RDONLY);
	if (data->infile == -1)
		error_fatal("infile", NULL);
	data->outfile = open(argv[argc - 1], O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (data->outfile == -1)
		error_fatal("outfile", NULL);
	data->here_doc_temp = NULL;
}

/**
 * close infile/outfile + all pipe fds as they are not used in main process
 * and otherwise child processes could not finish
 * 
 * pipe integer array is freed
 * 
 * in case of here_doc temporary file is unlinked and string to save temporary
 * file is freed
*/
void	cleanup(t_data *data)
{
	close(data->infile);
	close(data->outfile);
	close_unused_pipes(data);
	free_int_arr(data->pipes, data->pipe_count);
	if (data->here_doc_temp)
	{
		if (unlink(data->here_doc_temp) == -1)
			error_fatal("unlink of temp file failed", data);
	}
	free(data->here_doc_temp);
}

/**
 * HERE_DOC:
 * -STDIN-> -here_doc_temp-> [cmd1] <-pipe1-> ... <cmdn> -outfile->
 * first command is in argv at index 3
 * ncmds = argc - 4
 * output appended to outfile
 * 
 * NORMAL PIPING
 * -infile-> [cmd1] <-pipe1-> [cmd2] <-pipe2-> ... <cmdn> -outfile->
 * first command is in argv at index 2
 * ncmds = argc - 3
 * output overwrites existing data in outfile
 * 
 * pipe_manufacturing creates ncmds - 1 pipes
 * 
 * child_labor is executed ncmd times and executes the respective commands
 * by forking each time
 * 
 * cleanup closes all fds in main and discards here_doc_temp in case of
 * here_doc
 * 
 * main process waits ncmd times for all child processes to finish
*/
int	main(int argc, char **argv)
{
	int		i;
	t_data	data;

	if (argc < 5)
		usage();
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		i = 3;
		heredoc(&data, argv, argc);
	}
	else
	{
		i = 2;
		init_data(argv, argc, &data);
	}
	pipe_manufacturing(&data);
	while (i < argc - 1)
		child_labor(argv[i++], &data);
	cleanup(&data);
	while (data.cmd_count--)
		wait(NULL);
	return (EXIT_SUCCESS);
}
