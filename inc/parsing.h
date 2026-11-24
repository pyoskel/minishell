/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:48 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/24 06:17:15 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

t_token_type	get_token_type(const char *s);
t_token			*tokenize_words(const char *line);
t_token			*create_token(char *content);
void			add_token_back(t_token **head, t_token *new_node);
void			free_tokens(t_token *tokens);
void			print_tokens(t_token *tokens);
int				validate_syntax(t_token *tokens);

#endif