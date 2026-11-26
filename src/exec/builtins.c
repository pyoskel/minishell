/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 05:44:13 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:01:00 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int	builtin_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("pwd"), 1);
	ft_putendl_fd(cwd, 1);
	return (0);
}

static int	builtin_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strchr(envp[i], '='))
			ft_putendl_fd(envp[i], 1);
		i++;
	}
	return (0);
}

static int	builtin_unset(char **av, t_shell *shell)
{
	int	i;

	i = 1;
	while (av[i])
	{
		unset_env_value(shell, av[i]);
		i++;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **av, t_shell *shell)
{
	if (ft_strncmp(av[0], "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(av[0], "echo", 5) == 0)
		return (builtin_echo(av));
	if (ft_strncmp(av[0], "env", 4) == 0)
		return (builtin_env(shell->env));
	if (ft_strncmp(av[0], "exit", 5) == 0)
		return (builtin_exit(av, shell));
	if (ft_strncmp(av[0], "cd", 3) == 0)
		return (builtin_cd(av, shell));
	if (ft_strncmp(av[0], "unset", 6) == 0)
		return (builtin_unset(av, shell));
	if (ft_strncmp(av[0], "export", 7) == 0)
		return (builtin_export(av, shell));
	return (1);
}
