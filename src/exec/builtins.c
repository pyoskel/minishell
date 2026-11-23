/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 05:44:13 by esttina           #+#    #+#             */
/*   Updated: 2026/11/23 02:16:21 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int  is_n_flag(char *arg)
{
    int i;
    
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

static int  builtin_echo(char **av)
{
    int i;
    int newline;

    i = 1;
    newline = 1;
    while(av[i] && is_n_flag(av[i]))
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
static int  builtin_pwd(void)
{
    char    cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return (perror("pwd"), 1);
    ft_putendl_fd(cwd, 1);
    return (0);
}

int is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strncmp(cmd, "pwd", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "echo", 5) == 0)
        return (1);
    return (0);
}

int exec_builtin(char **av)
{
    if (ft_strncmp(av[0], "pwd", 4) == 0)
        return (builtin_pwd());
    if (ft_strncmp(av[0], "echo", 5) == 0)
        return (builtin_echo(av));
    return (1);
}
