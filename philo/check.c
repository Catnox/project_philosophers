/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 16:05:02 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 16:08:16 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	i = 0;
	while (str[i] == '0')
		i++;
	if (!str[i])
		return (0);
	if (ft_strlen(str + i) < 10)
		return (1);
	else if (ft_strlen(str + i) > 10)
		return (0);
	else
		return (ft_strcmp(str + i, "2147483647") <= 0);
}

static int	check_args(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!check_number(argv[i]))
			return (0);
		i++;
	}
	return (1);
}

int	validate_and_init(int argc, char **argv, t_data **data)
{
	if (argc < 5 || argc > 6 || !check_args(argv))
		return (write(STDERR_FILENO, "Error invalid\n", 14), 1);
	*data = init(*data, argc, argv);
	if (!*data)
		return (1);
	return (0);
}
