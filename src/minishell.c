/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/10/28 21:00:17 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

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