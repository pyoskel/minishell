/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/15 19:10:31 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/22 01:26:29 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

t_token *create_token(char *content)
{
	t_token	*node;

	if (!content)
		return (NULL);
	node = malloc(sizeof(t_token));
	if (!node)
		return (free(content), NULL);
	node->value = content;
	node->type = get_token_type(content);
	node->next = NULL;
	return (node);
}
void	add_token_back(t_token **head, t_token *new_node)
{
	t_token *last_node;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	last_node = *head;
	while (last_node->next)
		last_node = last_node->next;
	last_node->next = new_node;
}

void	free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}