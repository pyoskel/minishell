/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pabartoc <pabartoc@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:24:48 by pabartoc          #+#    #+#             */
/*   Updated: 2026/11/21 03:34:16 by pabartoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
}					t_token;

t_token	*tokenize_words(const char *line);
void	free_tokens(t_token *tokens);
void	print_tokens(t_token *tokens);

#endif