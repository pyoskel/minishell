/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:34 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 22:21:21 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

// after free_tokens(tokens);
// print_tokens(tokens); //for debugging

// void	print_tokens(t_token *tokens)
// {
// 	int	i;

// 	i = 0;
// 	while (tokens)
// 	{
// 		printf("Token %d (type %d): [%s]\n", i, tokens->type, tokens->value);
// 		tokens = tokens->next;
// 		i++;
// 	}
// }

// after free_tokens(tokens);
// print_cmds(cmds); //for debugging

// void	print_cmds(t_cmd *cmds)
// {
// 	int		c;
// 	int		i;
// 	t_redir	*r;

// 	c = 0;
// 	while (cmds)
// 	{
// 		printf("=== CMD %d ===\n", c++);
// 		i = 0;
// 		while (cmds->args && cmds->args[i])
// 		{
// 			printf("  arg[%d]: [%s]\n", i, cmds->args[i]);
// 			i++;
// 		}
// 		r = cmds->redirs;
// 		while (r)
// 		{
// 			printf("  redir (type %d): -> [%s]\n", r->type, r->target);
// 			r = r->next;
// 		}
// 		cmds = cmds->next;
// 	}
// }

static void	process_line(char *input, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (!input[0])
		return ;
	add_history(input);
	tokens = tokenize_words(input);
	if (tokens && validate_syntax(tokens))
	{
		expand_tokens(tokens, shell->env, shell->last_status);
		cmds = build_cmds(tokens);
		free_tokens(tokens);
		if (cmds && cmds->args && cmds->args[0])
			shell->last_status = exec_cmds(cmds, shell);
		free_cmds(cmds);
	}
	else if (tokens)
	{
		shell->last_status = 2;
		free_tokens(tokens);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)ac;
	(void)av;
	shell.env = dup_env(envp);
	shell.last_status = 0;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		process_line(input, &shell);
		free(input);
	}
	free_tab(shell.env);
	printf("\nExit\n");
	return (shell.last_status);
}
