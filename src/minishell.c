/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/14 20:10:10 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

// for testing: cc -Wall -Wextra -Werror minishell.c -lreadline

int	main(int ac, char **av, char **envp)
{
    char *input;
    char **args;
    
    (void)ac;
    (void)av;
    
    while (1)
    {
        input = readline("minishell$ ");
        if (!input) // Ctrl+D
            break;
        if (input[0] != '\0')
        {
            args = ft_split(input, ' ');
            exec_simple(args, envp);
            free_tab(args);
        }
        free(input);
    }
    printf("\nExit\n");
    return (0);
}