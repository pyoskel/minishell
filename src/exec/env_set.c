/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 03:01:00 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:15:35 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static char	*make_env_entry(char *name, char *value)
{
	char	*tmp;
	char	*entry;

	if (!value)
		return (ft_strdup(name));
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	return (entry);
}

static int	add_env_entry(t_shell *shell, char *entry)
{
	char	**new_env;
	int		i;

	i = 0;
	while (shell->env && shell->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (free(entry), 1);
	i = 0;
	while (shell->env && shell->env[i])
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[i] = entry;
	new_env[i + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
	return (0);
}

int	set_env_value(t_shell *shell, char *name, char *value)
{
	int		idx;
	char	*entry;

	entry = make_env_entry(name, value);
	if (!entry)
		return (1);
	idx = find_env_index(shell->env, name);
	if (idx >= 0)
	{
		free(shell->env[idx]);
		shell->env[idx] = entry;
		return (0);
	}
	return (add_env_entry(shell, entry));
}
