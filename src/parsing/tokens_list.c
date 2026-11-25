/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/15 19:10:31 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 19:58:49 by pabartoc         ###   ########.fr       */
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

static void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs->next;
		free(redirs->target);
		free(redirs);
		redirs = tmp;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		i = 0;
		while (cmds->args && cmds->args[i])
			free(cmds->args[i++]);
		free(cmds->args);
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}