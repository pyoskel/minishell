/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 17:57:04 by esttina           #+#    #+#             */
/*   Updated: 2026/11/23 03:06:10 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "parsing.h"

int     exec_simple(char **av, char **envp);
void    free_tab(char **tab);
char    **tokens_to_av(t_token *tokens);
int     is_builtin(char *cmd);
int     exec_builtin(char **av, char **envp);

#endif
