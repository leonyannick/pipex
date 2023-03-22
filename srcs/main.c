/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/22 22:07:05 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	init_data(t_data **data, int argc, char **argv)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
	{
		perror("data alloc failed");
		exit(EXIT_FAILURE);
	}
	(*data)->cmdc = argc -3;
	(*data)->argv = argv;
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;
	
	data = NULL;
	init_data(&data, argc, argv);
	// if (argc < 5)
	// 	return (ft_printf("not enough parameters\n"), EXIT_SUCCESS);
	
	if (pipe(data->fd) == -1)
		return (perror("pipe creation failed"), EXIT_FAILURE);
	printf("cmdc: %d\n", data->cmdc);
	// i = 0;
	// while (i < data->cmdc)
	// {
	data->pid = fork();
	if (data->pid == -1)
		return (perror("fork failed"), EXIT_FAILURE);
	if (data->pid == 0)
	{
		//child ->exec command
		dup2(data->fd[0], STDIN_FILENO);
		close(data->fd[0]);
		close(data->fd[1]);
		char str[10];
		read(STDIN_FILENO, str, 4);
		str[4] = 0;
		ft_putstr_fd(str, 2);
		
	}
	//parent
	close(data->fd[0]);
	const char test[] = "sometext";
	write(data->fd[1], &test, 8);
	close(data->fd[1]);
	data->wpid = waitpid(data->pid, NULL, 0);
	if (pipe(data->fd) == -1)
		return (perror("pipe creation failed"), EXIT_FAILURE);
	// 	i++;
	// }
	return (EXIT_SUCCESS);
}
