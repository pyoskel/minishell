/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 11:53:36 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:01:00 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int	exec_builtin_redir(t_cmd *cmd, t_shell *shell)
{
	int	saved_in;
	int	saved_out;
	int	status;

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

int	decode_status(int status)
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

void	run_child_cmd(t_cmd *cmd, t_shell *shell)
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

int	exec_cmds(t_cmd *cmds, t_shell *shell)
{
	if (!cmds)
		return (0);
	if (!cmds->next)
		return (exec_simple(cmds, shell));
	return (exec_pipeline(cmds, shell));
}
