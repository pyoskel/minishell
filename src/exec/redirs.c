/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:50:39 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 00:34:34 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static int  handle_heredoc(char *limiter)
{
    int     pfd[2];
    char    *line;
    size_t  len;

    if (pipe(pfd) == -1)
        return (perror("pipe"), -1);
    len = ft_strlen(limiter);
    while(1)
    {
        line = readline("> ");
        if (!line || ft_strncmp(line, limiter, len + 1) == 0)
        {
            free(line);
            break ;
        }
        ft_putendl_fd(line, pfd[1]);
        free(line);
    }
    close(pfd[1]);
    return (pfd[0]);
}

static int  open_redir_fd(t_redir *redir)
{
    if (redir->type == TOKEN_REDIR_IN)
        return (open(redir->target, O_RDONLY));
    if (redir->type == TOKEN_REDIR_OUT)
        return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
    if (redir->type == TOKEN_APPEND)
        return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
    if (redir->type == TOKEN_HEREDOC)
        return (handle_heredoc(redir->target));
    return (-1);
}

int apply_redirs(t_redir *redirs)
{
    int fd;
    int target_fd;

    while (redirs)
    {
        fd = open_redir_fd(redirs);
        if (fd == -1)
            return (perror(redirs->target), 1);
        if (redirs->type == TOKEN_REDIR_IN || redirs->type == TOKEN_HEREDOC)
            target_fd = 0;
        else
            target_fd = 1;
        dup2(fd, target_fd);
        close (fd);
        redirs = redirs->next;
    }
    return (0);
}