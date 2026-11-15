/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/15 19:19:16 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

// for testing: cc -Wall -Wextra -Werror minishell.c -lreadline

int	main(int ac, char **av, char **envp)
{
    char *input;
    char **args;
	t_token	*tokens; //for print_tokens (delete later)
    
    (void)ac;
    (void)av;
    
    while (1)
    {
        input = readline("minishell$ ");
		tokens = tokenize_words(input);
		print_tokens(tokens);
		free_tokens(tokens);
        if (!input) // Ctrl+D
            break;
        if (input[0] != '\0')
        {
            args = tokenize_words(input);
            exec_simple(args, envp);
            free_tab(args);
        }
        free(input);
    }
    printf("\nExit\n");
    return (0);
}

void	print_tokens(t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens)
	{
		printf("Token %d: [%s]\n", i, tokens->value);
		tokens = tokens->next;
		i++;
	}
}