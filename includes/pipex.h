/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:54 by lbaumann          #+#    #+#             */
/*   Updated: 2023/03/30 17:49:49 by lbaumann         ###   ########.fr       */
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

int	execute_cmd(char *cmd);

typedef struct s_data
{
	int		**pipes;
	int		pipe_count;
	int		ncmds;
	int		cmd_count;
	int		infile;
	int		outfile;
}t_data;

typedef enum e_redirect
{
	INPUT,
	OUTPUT,
	PIPE
}e_redirect;

#endif
