/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_time.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 15:33:47 by radubos           #+#    #+#             */
/*   Updated: 2025/08/22 19:25:52 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	precise_sleep(t_philo *philo, long duration)
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
		if (remaining > 200)
			usleep(100);
		else if (remaining > 20)
			usleep(10);
		else
			usleep(1);
	}
}

void	initial_delay(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->nb_philos > 1 && philo->id % 2 == 0)
		ft_usleep(data->time_to_eat / 2);
}
