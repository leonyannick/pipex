/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:53:28 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 12:09:00 by lbaumann         ###   ########.fr       */
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
