/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 16:35:01 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:01:00 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

char	**dup_env(char **envp)
{
	char	**copy;
	int		i;

	i = 0;
	while (envp && envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp && envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
			return (free_tab(copy), NULL);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	*get_env_value(char **env, char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	find_env_index(char **env, char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0
			&& (env[i][len] == '=' || env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

void	unset_env_value(t_shell *shell, char *name)
{
	int	idx;

	idx = find_env_index(shell->env, name);
	if (idx < 0)
		return ;
	free(shell->env[idx]);
	while (shell->env[idx + 1])
	{
		shell->env[idx] = shell->env[idx + 1];
		idx++;
	}
	shell->env[idx] = NULL;
}
