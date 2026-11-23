/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/22 05:44:13 by esttina           #+#    #+#             */
/*   Updated: 2026/11/23 23:07:57 by esttina          ###   ########.fr       */
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

static int  builtin_env(char **envp)
{
    int i;

    i = 0;
    while(envp && envp[i])
    {
        if (ft_strchr(envp[i], '='))
            ft_putendl_fd(envp[i], 1);
        i++;
    }
    return (0);
}

static int  is_numeric(char *s)
{
    int i;
    
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

static int  builtin_exit(char **av, t_shell *shell)
{
    int code;

    ft_putendl_fd("exit", 2);
    if (!av[1])
    {
        free_tab(av);
        free_tab(shell->env);
        exit(shell->last_status);
    }
    if (!is_numeric(av[1]))
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(av[1], 2);
        ft_putendl_fd(": numeric argument required", 2);
        free_tab(av);
        free_tab(shell->env);
        exit(2);
    }
    if (av[2])
        return (ft_putendl_fd("minishell: exit: too many arguments", 2), 1);
    code = ft_atoi(av[1]);
    free_tab(av);
    free_tab(shell->env);
    exit(code);
}

static int  builtin_cd(char **av, t_shell *shell)
{
    char    *target;
    
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
    if (chdir((target)) == -1)
        return (perror("minishell: cd"), 1);
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
    if (ft_strncmp(cmd, "env", 4) == 0)
        return (1);
    if (ft_strncmp(cmd, "exit", 5) == 0)
        return (1);
    if (ft_strncmp(cmd, "cd", 3) == 0)
        return (1);
    return (0);
}

int exec_builtin(char **av, t_shell *shell)
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
    return (1);
}
