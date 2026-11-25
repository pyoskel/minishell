/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 10:48:37 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 20:14:14 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	count_args(t_token *curr)
{
	int	count;
	count = 0;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type >= TOKEN_REDIR_IN)
			curr = curr->next->next;
		else
		{
			count++;
			curr = curr->next;
		}
	}
	return (count);
}

static int	add_redir(t_cmd *cmd, t_token *curr)
{
	t_redir	*node;
	t_redir	*last;
	node = malloc(sizeof(t_redir));
	if (!node)
		return (0);
	node->type = curr->type;
	node->target = ft_strdup(curr->next->value);
	if (!node->target)
		return (free(node), 0);
	node->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = node;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = node;
	}
	return (1);
}

static t_cmd	*init_cmd(t_token *curr)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = ft_calloc(count_args(curr) + 1, sizeof(char *));
	if (!cmd->args)
		return (free(cmd), NULL);
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	fill_cmd(t_cmd *cmd, t_token **curr)
{
	int	i;

	i = 0;
	while (*curr && (*curr)->type != TOKEN_PIPE)
	{
		if ((*curr)->type >= TOKEN_REDIR_IN)
		{
			if (!add_redir(cmd, *curr))
				return (0);
			*curr = (*curr)->next->next;
		}
		else
		{
			cmd->args[i] = ft_strdup((*curr)->value);
			if (!cmd->args[i])
				return (0);
			i++;
			*curr = (*curr)->next;
		}
	}
	return (1);
}

t_cmd	*build_cmds(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*last;
	t_cmd	*cmd;

	head = NULL;
	last = NULL;
	while (tokens)
	{
		cmd = init_cmd(tokens);
		if (!cmd || !fill_cmd(cmd, &tokens))
			return (free_cmds(cmd), free_cmds(head), NULL);
		if (!head)
			head = cmd;
		else
			last->next = cmd;
		last = cmd;
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (head);
}