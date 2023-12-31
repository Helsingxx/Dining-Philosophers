/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:29 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/31 11:52:44 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	kill_him(t_arg *arg, int times)
{
	int	x;

	x = 0;
	while (x < arg->nbr_philos
		&& !(!arg->timetodie || ((times - arg->lasttimeate[x])
				/ arg->timetodie >= 1
				&& ((arg->ate[x] != arg->to_be_eaten
						&& arg->to_be_eaten != -1)
					|| arg->to_be_eaten == -1))))
		x++;
	if (x != arg->nbr_philos)
	{
		gettimeofday(&arg->gtime2, NULL);
		printf("%ld %d has died\n", (arg->gtime2.tv_sec * 1000
				+ arg->gtime2.tv_usec / 1000)
			- (arg->gtime.tv_sec * 1000
				+ arg->gtime.tv_usec / 1000), x + 1);
		arg->exit = 1;
		pthread_mutex_unlock(&arg->lock_printf);
		return (FAIL);
	}
	return (LAP);
}

int	declare_eating(t_arg *args, int philo_id, int next)
{
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	args->lasttimeate[philo_id - 1] = (args->gtime2.tv_sec * 1000
			+ args->gtime2.tv_usec / 1000)
		- (args->gtime.tv_sec * 1000
			+ args->gtime.tv_usec / 1000);
	if (args->exit)
	{
		pthread_mutex_unlock(&args->lock_printf);
		pthread_mutex_unlock(&args->lock[philo_id - 1]);
		if (args->nbr_philos != 1)
			pthread_mutex_unlock(&args->lock[next - 1]);
		return (EXIT);
	}
	return (1);
}

int	eating(t_arg *args, int philo_id, int next)
{
	take_first_fork(args, philo_id);
	try_take_scnd(args, philo_id, next);
	if (declare_eating(args, philo_id, next) == EXIT)
		return (EXIT);
	printf("%ld %d is eating\n", (args->gtime2.tv_sec * 1000
			+ args->gtime2.tv_usec / 1000)
		- (args->gtime.tv_sec * 1000 + args->gtime.tv_usec / 1000), philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	chad_wastetime(args->timetoeat);
	pthread_mutex_unlock(&args->lock[philo_id - 1]);
	if (args->nbr_philos != 1)
		pthread_mutex_unlock(&args->lock[next - 1]);
	pthread_mutex_lock(&args->lock_printf);
	args->ate[philo_id - 1]++;
	if (args->ate[philo_id - 1] == args->to_be_eaten)
		return (pthread_mutex_unlock(&args->lock_printf), DONE);
	pthread_mutex_unlock(&args->lock_printf);
	return (LAP);
}

int	thinking(t_arg *args, int philo_id)
{
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (args->exit)
	{
		pthread_mutex_unlock(&args->lock_printf);
		return (EXIT);
	}
	printf("%ld %d is thinking\n", (args->gtime2.tv_sec * 1000
			+ args->gtime2.tv_usec / 1000)
		- (args->gtime.tv_sec * 1000
			+ args->gtime.tv_usec / 1000), philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	return (LAP);
}

int	sleeping(t_arg *args, int philo_id)
{
	pthread_mutex_lock(&args->lock_printf);
	if (args->exit)
	{
		pthread_mutex_unlock(&args->lock_printf);
		return (EXIT);
	}
	gettimeofday(&args->gtime2, NULL);
	printf("%ld %d is sleeping\n", (args->gtime2.tv_sec * 1000
			+ args->gtime2.tv_usec / 1000)
		- (args->gtime.tv_sec * 1000
			+ args->gtime.tv_usec / 1000), philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	chad_wastetime(args->timetosleep);
	return (0);
}
