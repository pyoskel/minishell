/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:23:19 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 19:52:45 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                             1. INCLUDES                                    */
/* ========================================================================== */

# include <unistd.h>
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include "libft.h"
# include "parsing.h"
# include <fcntl.h>

typedef struct  s_redir
{
    t_token_type    type;
    char            *target;
    struct s_redir  *next;
}   t_redir;

typedef struct  s_cmd
{
    char            **av;
    t_redir         *redirs;
    struct s_cmd    *next;
}   t_cmd;

/* ========================================================================== */
/*                     2. INTERNAL PARSER STRUCTURES                          */
/* ========================================================================== */

#endif