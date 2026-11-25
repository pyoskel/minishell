/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 11:53:36 by esttina           #+#    #+#             */
/*   Updated: 2026/11/25 21:20:34 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"
#include "parsing.h"

static char *get_path_env(char **envp)
{
    int i;

    i = 0;
    while(envp && envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

static char *find_path(char *cmd, char **envp)
{
    char **dirs;
    char *part_path;
    char *full_path;
    int i;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
    dirs = ft_split(get_path_env(envp), ':');
    if (!dirs)
        return (NULL);
    i = 0;
    while (dirs[i])
    {
        part_path = ft_strjoin(dirs[i], "/");
        full_path = ft_strjoin(part_path, cmd);
        free(part_path);
        if (full_path && access(full_path, X_OK) == 0)
            return (free_tab(dirs), full_path);
        free(full_path);
        i++;
    }
    return (free_tab(dirs), NULL);
}

int exec_simple(t_cmd *cmd, t_shell *shell)
{
    pid_t   pid;
    int     status;
    char    *path;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);
    if (is_builtin(cmd->args[0]))
        return (exec_builtin(cmd->args, shell));
    pid = fork();
    if (pid == -1)
        return (perror("fork"), 1);
    if (pid == 0)
    {
        path = find_path(cmd->args[0], shell->env);
        if (!path)
        {
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            exit(127);
        }
        if (apply_redirs(cmd->redirs) != 0)
        {
            free(path);
            exit(1);
        }
        execve(path, cmd->args, shell->env);
        perror(cmd->args[0]);
        free(path);
        exit(127); // command not found
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

void    free_tab(char **tab)
{
    int i;
    
    i = 0;
    if (!tab)
        return ;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}


