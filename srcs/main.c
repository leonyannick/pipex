/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/27 18:34:14 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	error_fatal(char *e_msg)
{
	perror(e_msg);
	exit(errno);
}

void	execute_cmd(char *cmd)
{
	char	str[10];
	read(STDIN_FILENO, str, 4);
	write(STDOUT_FILENO, str, 4);
}

void	create_child(char *cmd)
{
	pid_t	pid;
	int		fd[2];

	if (pipe(fd) == -1)
		error_fatal("pipe");
	pid = fork();
	if (pid == -1)
		error_fatal("fork");
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute_cmd(cmd);
		close(fd[1]);
		exit(1);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}

void	usage(void)
{
	ft_printf("not enough parameters\n");
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	int	i;
	int	infile;
	int	outfile;
	
	if (argc < 5)
		usage();
	
	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[argc - 1], O_RDWR);
	if (outfile == -1 || infile == -1)
		error_fatal("infile/outfile");
	dup2(infile, STDIN_FILENO);
	
	i = 2;
	while (i < argc - 2)
	{
		create_child(argv[i]);
		i++;
	}
	dup2(outfile, STDOUT_FILENO);
	execute_cmd(argv[argc - 2]);
	return (EXIT_SUCCESS);
}
