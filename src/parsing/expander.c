/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/24 08:14:36 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 02:37:30 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing.h"
#include "exec.h"

static char	*append_str(char *res, const char *add)
{
	char	*new_str;

	if (!res || !add)
		return (res);
	new_str = ft_strjoin(res, add);
	free(res);
	return (new_str);
}

static char	*append_char(char *res, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (append_str(res, buf));
}

static char	*handle_dollar(const char *s, int *i, char **env, int status)
{
	int		start;
	char	*name;
	char	*val;

	(*i)++;
	if (s[*i] == '?')
		return ((*i)++, ft_itoa(status));
	if (ft_isdigit(s[*i]))
		return ((*i)++, ft_strdup(""));
	if (!ft_isalpha(s[*i]) && s[*i] != '_')
		return (ft_strdup("$"));
	start = *i;
	while (ft_isalnum(s[*i]) || s[*i] == '_')
		(*i)++;
	name = ft_substr(s, start, *i - start);
	val = get_env_value(env, name);
	free(name);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

static char	*expand_word(const char *s, char **env, int status)
{
	char	*res;
	char	*tmp;
	int		i;
	char	quote;

	res = ft_strdup("");
	i = 0;
	quote = 0;
	while (s[i])
	{
		if (quote == 0 && (s[i] == '\'' || s[i] == '"'))
			quote = s[i++];
		else if (quote != 0 && s[i] == quote && ++i)
			quote = 0;
		else if (s[i] == '$' && quote != '\'')
		{
			tmp = handle_dollar(s, &i, env, status);
			res = append_str(res, tmp);
			free(tmp);
		}
		else
			res = append_char(res, s[i++]);
	}
	return (res);
}

void	expand_tokens(t_token *tokens, char **env, int status)
{
	t_token	*curr;
	char	*expanded;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD)
		{
			expanded = expand_word(curr->value, env, status);
			free(curr->value);
			curr->value = expanded;
		}
		curr = curr->next;
	}
}