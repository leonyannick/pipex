/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:54 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/31 12:13:21 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/includes/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>

extern char **environ;

# define READ 0
# define WRITE 1

typedef struct s_data
{
	int		**pipes;
	int		pipe_count;
	int		ncmds;
	int		cmd_count;
	int		infile;
	int		outfile;
}t_data;

int		execute_cmd(char *cmd, t_data *data);
void	error_fatal(char *e_msg, t_data *data);

//utils.c
void	free_int_arr(int **ptr, int nrows);

//pipex.c
void	child_labor(char *cmd, t_data *data);
void	plumbing(t_data *data);
void	close_unused_pipes(t_data *data);
void	pipe_manufacturing(t_data *data);

#endif
