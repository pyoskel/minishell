/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 03:01:00 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:15:24 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

int	builtin_cd(char **av, t_shell *shell)
{
	char	*target;
	char	old_cwd[4096];
	char	new_cwd[4096];
	int		has_old;

	if (av[1] && av[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	if (!av[1])
	{
		target = get_env_value(shell->env, "HOME");
		if (!target)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
	}
	else
		target = av[1];
	has_old = (getcwd(old_cwd, sizeof(old_cwd)) != NULL);
	if (chdir(target) == -1)
		return (perror("minishell: cd"), 1);
	if (has_old)
		set_env_value(shell, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, sizeof(new_cwd)) != NULL)
		set_env_value(shell, "PWD", new_cwd);
	return (0);
}

static int	is_valid_id(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	export_one(char *arg, t_shell *shell)
{
	char	*eq;
	char	*name;

	eq = ft_strchr(arg, '=');
	if (!eq)
	{
		if (!get_env_value(shell->env, arg))
			set_env_value(shell, arg, NULL);
		return ;
	}
	name = ft_substr(arg, 0, eq - arg);
	if (!name)
		return ;
	set_env_value(shell, name, eq + 1);
	free(name);
}

int	builtin_export(char **av, t_shell *shell)
{
	int	i;
	int	status;

	if (!av[1])
		return (print_sorted_export(shell->env));
	status = 0;
	i = 1;
	while (av[i])
	{
		if (!is_valid_id(av[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(av[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			export_one(av[i], shell);
		i++;
	}
	return (status);
}
