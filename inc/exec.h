/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 17:57:04 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:01:00 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "parsing.h"

typedef struct s_shell
{
	char	**env;
	int		last_status;
}	t_shell;

extern volatile sig_atomic_t	g_sig;

int		exec_cmds(t_cmd *cmds, t_shell *shell);
int		exec_simple(t_cmd *cmd, t_shell *shell);
int		exec_pipeline(t_cmd *cmds, t_shell *shell);
void	run_child_cmd(t_cmd *cmd, t_shell *shell);
int		decode_status(int status);
char	*find_path(char *cmd, char **envp);
void	free_tab(char **tab);
int		is_builtin(char *cmd);
int		exec_builtin(char **av, t_shell *shell);
int		builtin_echo(char **av);
int		builtin_exit(char **av, t_shell *shell);
int		builtin_cd(char **av, t_shell *shell);
int		builtin_export(char **av, t_shell *shell);
int		print_sorted_export(char **env);
char	**dup_env(char **envp);
char	*get_env_value(char **env, char *name);
int		find_env_index(char **env, char *name);
int		set_env_value(t_shell *shell, char *name, char *value);
void	unset_env_value(t_shell *shell, char *name);
int		apply_redirs(t_redir *redirs);
void	setup_signals_prompt(void);
void	setup_signals_wait(void);
void	setup_signals_child(void);

#endif
