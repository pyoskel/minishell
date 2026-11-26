/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:48 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/25 22:20:57 by pabartoc         ###   ########.fr       */
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

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}					t_cmd;

t_token_type	get_token_type(const char *s);
t_token			*tokenize_words(const char *line);
t_token			*create_token(char *content);
void			add_token_back(t_token **head, t_token *new_node);
void			free_tokens(t_token *tokens);
int				validate_syntax(t_token *tokens);
void			expand_tokens(t_token *tokens, char **env, int status);
t_cmd			*build_cmds(t_token *tokens);
void			free_cmds(t_cmd *cmds);
// void			print_tokens(t_token *tokens); // for debugging
// void			print_cmds(t_cmd *cmds); // for debugging

#endif