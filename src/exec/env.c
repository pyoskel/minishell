/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/23 16:35:01 by esttina           #+#    #+#             */
/*   Updated: 2026/11/23 23:13:48 by esttina          ###   ########.fr       */
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