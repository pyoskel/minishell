/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:26:33 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/22 01:57:46 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

static int	operator_len(const char *s)
{
	if ((s[0] == '<' && s[1] == '<') || (s[0] == '>' && s[1] == '>'))
		return (2);
	if (s[0] == '|' || s[0] == '<' || s[0] == '>')
		return (1);
	return (0);
}

t_token_type	get_token_type(const char *s)
{
	if (s[0] == '<' && s[1] == '<')
		return (TOKEN_HEREDOC);
	if (s[0] == '>' && s[1] == '>')
		return (TOKEN_APPEND);
	if (s[0] == '|')
		return (TOKEN_PIPE);
	if (s[0] == '<')
		return (TOKEN_REDIR_IN);
	if (s[0] == '>')
		return (TOKEN_REDIR_OUT);
	return (TOKEN_WORD);
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
		if (operator_len(&line[i]) > 0)
			i += operator_len(&line[i]);
		else
		{
			while (line[i] && !is_space(line[i]) && !operator_len(&line[i]))
				i++;
		}
		node = create_token(ft_substr(line, start, i - start));
		if (!node)
			return (free_tokens(head), NULL);
		add_token_back(&head, node);
	}
	return (head);
}
