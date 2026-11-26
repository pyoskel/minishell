/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 11:53:36 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 01:59:07 by esttina          ###   ########.fr       */
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

static int  exec_builtin_redir(t_cmd *cmd, t_shell *shell)
{
    int saved_in;
    int saved_out;
    int status;

    if (!cmd->redirs)
        return (exec_builtin(cmd->args, shell));
    saved_in = dup(0);
    saved_out = dup(1);
    status = 1;

    if (apply_redirs(cmd->redirs) == 0)
    {
        if (cmd->args && cmd->args[0])
            status = exec_builtin(cmd->args, shell);
        else
            status = 0;
    }
    dup2(saved_in, 0);
    dup2(saved_out, 1);
    close(saved_in);
    close(saved_out);
    return (status);
}

static int	decode_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit: 3", 2);
		return (128 + WTERMSIG(status));
	}
	return (1);
}

static void	run_child_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (apply_redirs(cmd->redirs) != 0)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd->args, shell));
	path = find_path(cmd->args[0], shell->env);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->args, shell->env);
	perror(cmd->args[0]);
	free(path);
	if (errno == EACCES || errno == EISDIR)
		exit(126);
	exit(127);
}

int	exec_simple(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd)
		return (0);
	if (!cmd->args || !cmd->args[0])
	{
		if (!cmd->redirs)
			return (0);
		return (exec_builtin_redir(cmd, shell));
	}
	if (is_builtin(cmd->args[0]))
		return (exec_builtin_redir(cmd, shell));
	setup_signals_wait();
	pid = fork();
	if (pid == -1)
		return (setup_signals_prompt(), perror("fork"), 1);
	if (pid == 0)
	{
		setup_signals_child();
		run_child_cmd(cmd, shell);
	}
	waitpid(pid, &status, 0);
	setup_signals_prompt();
	return (decode_status(status));
}

static int	wait_pipeline(pid_t last_pid)
{
	int	status;
	int	last_status;

	last_status = 0;
	if (last_pid > 0 && waitpid(last_pid, &status, 0) > 0)
		last_status = decode_status(status);
	while (wait(NULL) > 0)
		;
	setup_signals_prompt();
	return (last_status);
}

static int	exec_pipeline(t_cmd *cmds, t_shell *shell)
{
	int		pfd[2];
	int		prev_fd;
	pid_t	pid;

	prev_fd = -1;
	pid = -1;
	setup_signals_wait();
	while (cmds)
	{
		if (cmds->next && pipe(pfd) == -1)
			return (setup_signals_prompt(), perror("pipe"), 1);
		pid = fork();
		if (pid == 0)
		{
			setup_signals_child();
			if (prev_fd != -1)
				(dup2(prev_fd, 0), close(prev_fd));
			if (cmds->next)
				(dup2(pfd[1], 1), close(pfd[0]), close(pfd[1]));
			run_child_cmd(cmds, shell);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds->next)
			(close(pfd[1]), prev_fd = pfd[0]);
		cmds = cmds->next;
	}
	return (wait_pipeline(pid));
}

int exec_cmds(t_cmd *cmds, t_shell *shell)
{
    if (!cmds)
        return (0);
    if (!cmds->next)
        return (exec_simple(cmds, shell));
    return (exec_pipeline(cmds, shell));
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


