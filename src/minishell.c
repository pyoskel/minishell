/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/22 01:37:57 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

// for testing: cc -Wall -Wextra -Werror minishell.c -lreadline
// , char **envp
int	main(int ac, char **av)
{
    char *input;
    // char **args;
	t_token	*tokens; //for print_tokens (delete later)
    
    (void)ac;
    (void)av;
    
    while (1)
    {
        input = readline("minishell$ ");
        if (!input) // Ctrl+D
			break;
		tokens = tokenize_words(input);
		print_tokens(tokens);
		free_tokens(tokens);
        // if (input[0] != '\0')
        // {
        //     args = ft_split(input, ' ');
        //     exec_simple(args, envp);
        //     free_tab(args);
        // }
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
		printf("Token %d (type %d): [%s]\n", i, tokens->type, tokens->value);
		tokens = tokens->next;
		i++;
	}
}
