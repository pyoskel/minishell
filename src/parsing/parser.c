/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:26:33 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/15 19:43:19 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	is_space(char c)
{
	return (c == ' ');
}

static t_token *create_token(char *content)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = content;
	node->next = NULL;
	return (node);
}

static void	add_token_back(t_token **head, t_token *new_node)
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

t_token	*tokenize_words(const char *line)
{
	t_token	*head;
	t_token	*node;
	int		i;
	int		start;

	head = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		start = i;
		while (line[i] && !is_space(line[i]))
			i++;
		node = create_token(ft_substr(line, start, i - start));
		if (!node)
			return (free(node), free_tokens(head), NULL);
		add_token_back(&head, node);
	}
	return (head);
}
