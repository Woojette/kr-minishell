/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_exit_ft.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wooyang <wooyang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:44:27 by wooyang           #+#    #+#             */
/*   Updated: 2025/05/14 15:55:42 by wooyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit_check_not_int(char *str)
{
	int	i;

	i = 0;
	if ((str[0] == '+') || (str[0] == '-'))
		i++;
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == 1)
			return (1);
		i++;
	}
	return (0);
}

long long	ft_exit_atoi_long(const char *str, int *error)
{
	int			i;
	int			signe;
	long long	nb;

	i = 0;
	signe = 1;
	nb = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			signe = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (signe == 1 && (nb > (LLONG_MAX - (str[i] - '0')) / 10))
			return (*error = 1, LLONG_MAX);
		if (signe == -1 && (-nb < (LLONG_MIN + (str[i] - '0')) / 10))
			return (*error = 1, LLONG_MIN);
		nb = (nb * 10) + (str[i] - '0');
		i++;
	}
	return (nb * signe);
}

void	exit_clean(t_mini *mini)
{
	t_m	*m;

	m = (t_m *)mini->m_ptr;
	if (m)
	{
		if (m->parsing)
			free_tokens(&m->parsing);
		if (m->line)
			free(m->line);
		m->parsing = NULL;
		m->line = NULL;
	}
	termios_back(mini);
	free_mini(mini);
}
