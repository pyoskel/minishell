/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 17:57:04 by esttina           #+#    #+#             */
/*   Updated: 2026/11/24 05:13:23 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "parsing.h"

typedef struct s_shell
{
    char **env;
    int last_status;
}   t_shell;

int     exec_simple(char **av, t_shell *shell);
void    free_tab(char **tab);
int     is_builtin(char *cmd);
int     exec_builtin(char **av, t_shell *shell);
char    **dup_env(char **envp);
char    *get_env_value(char **env, char *name);
int     set_env_value(t_shell *shell, char *name, char *value);

#endif
