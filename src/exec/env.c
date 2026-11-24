/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 16:35:01 by esttina           #+#    #+#             */
/*   Updated: 2026/11/24 05:05:51 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

char **dup_env(char **envp)
{
    char    **copy;
    int     i;

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

char    *get_env_value(char **env, char *name)
{
    int     i;
    size_t  len;

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

static int  find_env_index(char **env, char *name)
{
    int     i;
    size_t  len;

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

static char *make_env_entry(char *name, char *value)
{
    char    *tmp;
    char    *entry;

    if (!value)
        return (ft_strdup(name));
    tmp = ft_strjoin(name, "=");
    if (!tmp)
        return (NULL);
    entry = ft_strjoin(tmp, value);
    free(tmp);
    return (entry);
}

static int  add_env_entry(t_shell *shell, char *entry)
{
    char    **new_env;
    int     i;

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

int set_env_value(t_shell *shell, char *name, char *value)
{
    int     idx;
    char    *entry;

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