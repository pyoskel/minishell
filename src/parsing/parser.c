/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:26:33 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/23 06:44:49 by pabartoc         ###   ########.fr       */
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

static int	word_len_quote(const char *s)
{
	int len;
	char quote;

	len = 0;
	quote = 0;
	while (s[len])
	{
		if (quote == 0 && (is_space(s[len]) || operator_len(&s[len]) > 0))
			break ;
		if (quote == 0 && (s[len] == '\'' || s[len] == '"'))
			quote = s[len];
		else if (quote != 0 && s[len] == quote)
			quote = 0;
		len++;
	}
	if (quote != 0)
		return (-1);
	return(len);
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
			if (word_len_quote(&line[i]) < 0)
			{
				printf("minishell: syntax error: unclosed quote\n");
				return (free_tokens(head), NULL);
			}
				i+= word_len(&line[i]);
		}
		node = create_token(ft_substr(line, start, i - start));
		if (!node)
			return (free_tokens(head), NULL);
		add_token_back(&head, node);
	}
	return (head);
}
