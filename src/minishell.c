/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/24 05:34:58 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

int	main(int ac, char **av, char **envp)
{
    char *input;
    char **args;
	t_token	*tokens; //for print_tokens (delete later)
    t_shell shell;
    
    (void)ac;
    (void)av;
    
    shell.env = dup_env(envp);
    shell.last_status = 0;
    while (1)
    {
        input = readline("minishell$ ");
        if (!input) // Ctrl+D
			break;
        tokens = tokenize_words(input);
        free(input);
        print_tokens(tokens);
        args = tokens_to_av(tokens);
        free_tokens(tokens);
        if (args && args[0])
            shell.last_status = exec_simple(args, &shell);
        free_tab(args);
        // if (input[0] != '\0')
        // {
        //     args = ft_split(input, ' ');
        //     exec_simple(args, envp);
        //     free_tab(args);
        // }
    }
    free_tab(shell.env);
    printf("\nExit\n");
    return (shell.last_status);
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
