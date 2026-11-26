/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 03:01:00 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:15:43 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

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

static pid_t	fork_pipe_cmd(t_cmd *cmd, t_shell *shell, int prev_fd, int *pfd)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_signals_child();
		if (prev_fd != -1)
		{
			dup2(prev_fd, 0);
			close(prev_fd);
		}
		if (cmd->next)
		{
			dup2(pfd[1], 1);
			close(pfd[0]);
			close(pfd[1]);
		}
		run_child_cmd(cmd, shell);
	}
	return (pid);
}

int	exec_pipeline(t_cmd *cmds, t_shell *shell)
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
		pid = fork_pipe_cmd(cmds, shell, prev_fd, pfd);
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds->next)
		{
			close(pfd[1]);
			prev_fd = pfd[0];
		}
		cmds = cmds->next;
	}
	return (wait_pipeline(pid));
}
