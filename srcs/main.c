/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:52:40 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/22 15:34:03 by lbaumann         ###   ########.fr       */
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


void    print_string_arr(char **arr)
{
    if(arr == 0)
    {
        printf("Empty arr\n");
        return;
    }
    while(*arr != 0)
    {
        printf("%s\n", *arr);
        arr++;
    }
}

int	main(int argc, char **argv)
{
	t_data	*data;
	int		i;
	
	data = NULL;
	init_data(&data, argc, argv);
	if (argc < 5)
		return (ft_printf("not enough parameters\n"), EXIT_SUCCESS);
	
	if (pipe(data->fd) == -1)
		return (perror("pipe creation failed"), EXIT_FAILURE);
	printf("cmdc: %d\n", data->cmdc);
	i = 0;
	
	print_string_arr(environ);
	/* while (i < data->cmdc)
	{
		data->pid = fork();
		if (data->pid == -1)
			return (perror("fork failed"), EXIT_FAILURE);
		if (data->pid > 0)
		{
			//parent ->exec command
			close(data->fd[0]);
			dup2(data->fd[1], STDOUT_FILENO);
			close(data->fd[1]);
			execve("ls", NULL, NULL);
		}
		else
		{
			//child ->create new pipe
			if (pipe(data->fd) == -1)
				return (perror("pipe creation failed"), EXIT_FAILURE);
		}
		i++;
	} */
	return (EXIT_SUCCESS);
}
