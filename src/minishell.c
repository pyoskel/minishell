/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 04:26:16 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

int	main(int ac, char **av, char **envp)
{
    char *input;
    char **args;
	t_token	*tokens;
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
        if (tokens && validate_syntax(tokens))
		{
			expand_tokens(tokens, shell.env, shell.last_status);
			print_tokens(tokens); //for debugging (delete later)
			args = tokens_to_av(tokens);
			free_tokens(tokens);
			if (args && args[0])
				shell.last_status = exec_simple(args, &shell);
			free_tab(args);
		}
        else if (tokens)
		{
			shell.last_status = 2;
			free_tokens(tokens);
		}
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
