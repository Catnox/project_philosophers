/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_time.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:33:47 by radubos           #+#    #+#             */
/*   Updated: 2025/08/21 16:23:03 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	adaptive_usleep(long remaining)
{
	if (remaining > 1000)
		usleep(1000);
	else if (remaining > 100)
		usleep(remaining / 2);
	else
		usleep(50);
}

static void	precise_sleep(t_philo *philo, long duration)
{
	long	start_time;
	long	elapsed;
	long	remaining;

	start_time = get_current_time();
	while (1)
	{
		if (check_death_during_sleep(philo))
			return ;
		elapsed = get_current_time() - start_time;
		if (elapsed >= duration)
			break ;
		remaining = duration - elapsed;
		adaptive_usleep(remaining);
	}
}

void	initial_delay(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->nb_philos == 2 && philo->id % 2 == 0)
		usleep(data->time_to_eat * 500);
	else if (philo->id % 2 == 0)
		usleep(1000);
}
