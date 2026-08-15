/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xingchen <xingchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 14:00:00 by yushan            #+#    #+#             */
/*   Updated: 2026/08/16 02:47:39 by xingchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	is_digit(char character)
{
	return (character >= '0' && character <= '9');
}

static int	accumulate_number(char *argument, int *index, int sign,
	unsigned long long *value)
{
	unsigned long long	limit;

	limit = LLONG_MAX;
	if (sign < 0)
		limit++;
	*value = 0;
	while (is_digit(argument[*index]))
	{
		if (*value > (limit - (argument[*index] - '0')) / 10)
			return (0);
		*value = *value * 10 + argument[(*index)++] - '0';
	}
	return (1);
}

static int	parse_exit_number(char *argument, long long *number)
{
	unsigned long long	value;
	int					i;
	int					sign;

	i = 0;
	while (is_space(argument[i]))
		i++;
	sign = 1;
	if (argument[i] == '+' || argument[i] == '-')
		if (argument[i++] == '-')
			sign = -1;
	if (!is_digit(argument[i]))
		return (0);
	if (!accumulate_number(argument, &i, sign, &value))
		return (0);
	while (is_space(argument[i]))
		i++;
	if (argument[i])
		return (0);
	if (sign < 0 && value == (unsigned long long)LLONG_MAX + 1)
		*number = LLONG_MIN;
	else
		*number = (long long)value * sign;
	return (1);
}

static int	exit_numeric_error(char *argument)
{
	write(2, "minishell: exit: ", 17);
	write(2, argument, ft_strlen(argument));
	write(2, ": numeric argument required\n", 28);
	return (2);
}

int	builtin_exit(t_shell *shell, t_cmd *cmd)
{
	long long	number;
	int			status;

	if (!cmd->argv[1])
	{
		shell->should_exit = 1;
		return (shell->exit_status);
	}
	if (!parse_exit_number(cmd->argv[1], &number))
	{
		shell->should_exit = 1;
		return (exit_numeric_error(cmd->argv[1]));
	}
	if (cmd->argv[2])
		return (write(2, "minishell: exit: too many arguments\n", 36), 1);
	status = number % 256;
	if (status < 0)
		status += 256;
	shell->should_exit = 1;
	return (status);
}
