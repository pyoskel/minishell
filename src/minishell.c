/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/22 03:22:33 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

// for testing: cc -Wall -Wextra -Werror minishell.c -lreadline
// , char **envp
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
        if (!input) // Ctrl+D
			break;
        tokens = tokenize_words(input);
        print_tokens(tokens);
        args = tokens_to_av(tokens);
        free_tokens(tokens);
        if (args && args[0])
            exec_simple(args, envp);
        free_tab(args);
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
		printf("Token %d: [%s]\n", i, tokens->value);
		tokens = tokens->next;
		i++;
	}
}