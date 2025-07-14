/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: namichel <marvin@d42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 04:45:15 by namichel          #+#    #+#             */
/*   Updated: 2025/06/24 04:45:15 by namichel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	ft_atoi(const char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != 0)
		return (-1);
	return (res);
}

static int	check_inputs(t_data *data, int argc, char **argv)
{
	char	*e;

	e = "./philo nb_philos time_to_die time_to_eat time_to_sleep [max_meals]\n";
	if (argc != 5 && argc != 6)
	{
		write(2, e, ft_strlen(e));
		return (1);
	}
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->max_meals = ft_atoi(argv[5]);
	else
		data->max_meals = -1;
	data->someone_died = 0;
	data->start_time = get_time();
	if (data->nb_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (argc == 6 && data->max_meals <= 0))
	{
		write(2, "Invalid arguments(s)\n", 21);
		return (1);
	}
	return (0);
}

int	init_data(t_data *data, int argc, char **argv, int i)
{
	if (check_inputs(data, argc, argv) != 0)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
		return (write(2, "Malloc forks failed\n", 20), 1);
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			cleanup(data, 0, --i);
			return (1);
		}
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		cleanup(data, 0, data->nb_philos);
		return (1);
	}
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		cleanup(data, 0, data->nb_philos);
		return (1);
	}
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		cleanup(data, 0, data->nb_philos);
		return (write(2, "Malloc philos failed\n", 21), 1);
	}
	while (i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
		i++;
	}
	return (0);
}
