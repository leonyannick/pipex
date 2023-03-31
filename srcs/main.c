/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 16:27:27 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	error_fatal(char *e_msg, t_data *data)
{
	if (data)
		free_int_arr(data->pipes, data->pipe_count);
	perror(e_msg);
	exit(errno);
}

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
	data->outfile = open(argv[argc - 1], O_RDWR);
	data->here_doc_temp = NULL;
	if (data->outfile == -1 || data->infile == -1)
		error_fatal("infile/outfile", NULL);
}

void	cleanup(t_data *data)
{
	close(data->infile);
	close(data->outfile);
	close_unused_pipes(data);
	free_int_arr(data->pipes, data->pipe_count);
	if (data->here_doc_temp)
		unlink(data->here_doc_temp);
	free(data->here_doc_temp);
}

void	heredoc(t_data *data, char **argv, int argc)
{
	char	*line;
	
	data->here_doc_temp = ft_strdup("./.here_doc_temp");
	data->infile = open(data->here_doc_temp, O_CREAT | O_RDWR, 0666);
	if (data->infile == -1)
		error_fatal("here_doc creation failed", NULL);
	line = get_next_line(STDIN_FILENO);
	while (ft_strncmp(line, argv[2], ft_strlen(argv[2]))
		|| line[ft_strlen(argv[2])] != '\n')
	{
		write(data->infile, line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	close(data->infile);
	data->infile = data->infile = open("./.here_doc_temp", O_RDONLY);
	if (data->infile == -1)
		error_fatal("could not open here_doc_temp", NULL);
	data->ncmds = argc - 4;
	data->cmd_count = 0;
	data->outfile = open(argv[argc - 1], O_RDWR | O_APPEND);
	if (data->outfile == -1)
		error_fatal("outfile", NULL);
}

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
