/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 03:01:00 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:15:27 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **av)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (av[i] && is_n_flag(av[i]))
	{
		newline = 0;
		i++;
	}
	while (av[i])
	{
		ft_putstr_fd(av[i], 1);
		if (av[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', 1);
	return (0);
}

static int	is_numeric(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	exit_numeric_err(char **av, t_shell *shell)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(av[1], 2);
	ft_putendl_fd(": numeric argument required", 2);
	free_tab(av);
	free_tab(shell->env);
	exit(2);
}

int	builtin_exit(char **av, t_shell *shell)
{
	int	code;

	ft_putendl_fd("exit", 2);
	if (!av[1])
	{
		free_tab(av);
		free_tab(shell->env);
		exit(shell->last_status);
	}
	if (!is_numeric(av[1]))
		exit_numeric_err(av, shell);
	if (av[2])
		return (ft_putendl_fd("minishell: exit: too many arguments", 2), 1);
	code = ft_atoi(av[1]);
	free_tab(av);
	free_tab(shell->env);
	exit(code);
}
