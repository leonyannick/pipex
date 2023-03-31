/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 16:31:58 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 17:15:41 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

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
	data->infile = open("./.here_doc_temp", O_RDONLY);
	if (data->infile == -1)
		error_fatal("could not open here_doc_temp", NULL);
	data->ncmds = argc - 4;
	data->cmd_count = 0;
	data->outfile = open(argv[argc - 1], O_RDWR | O_APPEND);
	if (data->outfile == -1)
		error_fatal("outfile", NULL);
}
