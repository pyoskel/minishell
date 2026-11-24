/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 04:40:58 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/24 05:13:21 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	syntax_error(const char *token_val)
{
	printf("minishell: syntax error unexpected token '%s'\n", token_val);
	return (0);
}

int	validate_syntax(t_token *tokens)
{
	t_token	*curr;

	if (!tokens)
		return (1);
	if (tokens->type == TOKEN_PIPE)
		return (syntax_error(tokens->value));
	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE
			&& (!curr->next || curr->next->type == TOKEN_PIPE))
			return (syntax_error("|"));
		if (curr->type >= TOKEN_REDIR_IN && !curr->next)
			return(syntax_error("newline"));
		if (curr->type >= TOKEN_REDIR_IN && curr->next->type != TOKEN_WORD)
			return (syntax_error(curr->next->value));
		curr = curr->next;
	}
	return (1);
}
