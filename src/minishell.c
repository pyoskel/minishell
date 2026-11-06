/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/06 19:57:26 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

// for testing: cc -Wall -Wextra -Werror minishell.c -lreadline

int	main(void)
{
    char *input;
    while (1)
    {
        input = readline("minishell$ ");
        if (!input) // Ctrl+D
            break;
        if (input[0] != '\0')
            printf("It works! You typed: %s\n", input);
        free(input);
    }
    printf("\nExit\n");
    return (0);
}