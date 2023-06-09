/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbaumann <lbaumann@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 10:00:48 by lbaumann          #+#    #+#             */
/*   Updated: 2023/04/04 10:40:31 by lbaumann         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

/**
 * goes through path variable and searches for the program in its directories
 * if it exists and is executable it calls execve and transforms the calling
 * process into the program
 * 
 * @param cmd name of command to be executed
 * 
 * goes through all substrings of global variable environ to find PATH variable
 * splits PATH into paths using split at ':'
 * splits cmd with ' ' to seperate program and arguments
 * joins program name with '/'
 * tests all paths joined with program name with access
 * if write access is available execve is called -> takes over memory
 * memory is freed if program doesnt exist
*/
static void	search_cmd_in_path(char *cmd, char **envp, t_data *data)
{
	int		i;
	char	**paths;

	i = 0;
	while (ft_strncmp(environ[i], "PATH", 4))
		i++;
	paths = ft_split(&environ[i][4], ':');
	i = 0;
	cmd = ft_strjoin("/", envp[0]);
	free(envp[0]);
	envp[0] = ft_strjoin(paths[i], cmd);
	while (access(envp[0], X_OK) && paths[i])
	{
		free(envp[0]);
		envp[0] = ft_strjoin(paths[i], cmd);
		i++;
	}
	if (access(envp[0], X_OK) == 0)
		execve(envp[0], envp, NULL);
	error_fatal(cmd, data);
	free(cmd);
	ft_free_split_arr(envp);
	ft_free_split_arr(paths);
}

/**
 * stops in case of an empty string (otherwise access would fail in bad access)
 * then checks if cmd is already a valid executable
 * if not function search_cmd_in_path is called to search for the cmd in path
 * environment variable
*/
void	execute_cmd(char *cmd, t_data *data)
{
	char	**envp;

	if (!cmd[0])
		error_fatal("cmd is an empty str", data);
	envp = ft_split(cmd, ' ');
	if (access(envp[0], X_OK) == 0)
		execve(envp[0], envp, NULL);
	search_cmd_in_path(cmd, envp, data);
}
