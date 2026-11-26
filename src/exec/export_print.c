/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esttina <esttina@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/26 03:01:00 by esttina           #+#    #+#             */
/*   Updated: 2026/11/26 03:15:38 by esttina          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exec.h"

static void	print_export_line(char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	ft_putstr_fd("declare -x ", 1);
	if (!eq)
	{
		ft_putendl_fd(entry, 1);
		return ;
	}
	write(1, entry, eq - entry);
	ft_putstr_fd("=\"", 1);
	ft_putstr_fd(eq + 1, 1);
	ft_putendl_fd("\"", 1);
}

static void	sort_tab(char **tab)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (tab[i])
	{
		j = 0;
		while (tab[j + 1])
		{
			if (ft_strncmp(tab[j], tab[j + 1], ft_strlen(tab[j]) + 1) > 0)
			{
				tmp = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	print_sorted_export(char **env)
{
	char	**copy;
	int		i;

	i = 0;
	while (env && env[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (1);
	i = -1;
	while (env && env[++i])
		copy[i] = env[i];
	copy[i] = NULL;
	sort_tab(copy);
	i = 0;
	while (copy[i])
		print_export_line(copy[i++]);
	free(copy);
	return (0);
}
