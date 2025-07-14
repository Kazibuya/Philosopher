/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namichel <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 01:27:00 by namichel          #+#    #+#             */
/*   Updated: 2025/06/27 01:27:00 by namichel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

void	precise_sleep(long long time_ms)
{
	long long	start;

	start = get_time();
	while (get_time() - start < time_ms)
		usleep(100);
}

void	print_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	if (!philo->data->someone_died)
		printf("%lld %d %s\n", get_time() - philo->data->start_time, philo->id,
			action);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
