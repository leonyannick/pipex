/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:53:28 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 17:38:46 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/**
 * @param e_msg error message to be printed on STDERR with perror()
 * @param data pointer to data struct to free allocated memory 
 * (pass NULL if there is no data allocated)
 * 
 * exits the program with the corresponding errno
*/
void	error_fatal(char *e_msg, t_data *data)
{
	if (data)
	{
		free_int_arr(data->pipes, data->pipe_count);
		if (data->here_doc_temp)
			free(data->here_doc_temp);
	}
	perror(e_msg);
	exit(errno);
}

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
