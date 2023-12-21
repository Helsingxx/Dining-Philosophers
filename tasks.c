/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:29 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/20 17:35:54 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int kill_him(t_arg *arg, int times)
{
	int x;

	x = 0;
	while (x < arg->nbr_philos
		&& !(!arg->timetodie || (arg->ate[x] < times 
		&& ((arg->ate[x] != arg->to_be_eaten
		&& arg->to_be_eaten != -1)
		|| arg->to_be_eaten == -1))))
		x++;
	if (x != arg->nbr_philos)
	{
		printf("%d %d has died %d - %d\n", times * arg->timetodie, x + 1, arg->ate[x], times);
		arg->exit = 1;
		pthread_mutex_unlock(&arg->lock_printf);
		return (FAIL);
	}
	return (LAP);
}

int eating(t_arg *args, int philo_id, int next)
{
	int forks;

	forks = 0;

	pthread_mutex_lock(&(args->lock[philo_id - 1]));
	gettimeofday(&args->gtime2, NULL);
	pthread_mutex_lock(&args->lock_printf);
	if (!args->exit)
		printf("%ld %d has taken a fork\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	if (args->nbr_philos != 1)
		pthread_mutex_lock(&args->lock[next - 1]);
	else
		chad_wastetime(args->timetodie); // if there is one philosopher
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (!args->exit)
		printf("%ld %d has taken a fork\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 +
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	//pthread_mutex_lock(&args->lock[args->nbr_philos]);
	pthread_mutex_lock(&args->lock_printf);
	args->ate[philo_id - 1]++;
	gettimeofday(&args->gtime2, NULL);
	if (args->exit)
	{
		pthread_mutex_unlock(&args->lock_printf);
		pthread_mutex_unlock(&args->lock[philo_id - 1]);
		if (args->nbr_philos != 1)
			pthread_mutex_unlock(&args->lock[next - 1]);
		return (EXIT);
	}
	printf("%ld %d is eating\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
								(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	//pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	pthread_mutex_unlock(&args->lock_printf);
	chad_wastetime(args->timetoeat);
	pthread_mutex_unlock(&args->lock[philo_id - 1]);
	if (args->nbr_philos != 1)
		pthread_mutex_unlock(&args->lock[next - 1]);
	if (args->ate[philo_id - 1] == args->to_be_eaten)
		return (DONE);
	return (LAP);
}

int thinking(t_arg *args, int philo_id)
{	
//	pthread_mutex_lock(&args->lock[args->nbr_philos]);
	;
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (args->exit)
	{
	//	pthread_mutex_unlock(&args->lock[args->nbr_philos]);
		pthread_mutex_unlock(&args->lock_printf);
		return (EXIT);
	}
	printf("%ld %d is thinking\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
								(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
//	pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	pthread_mutex_unlock(&args->lock_printf);
//	pthread_mutex_lock(&args->lock[args->nbr_philos]);
	//if (args->exit)
	//{
	//	pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	//	return (EXIT);
	//}
//	pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	return (LAP);
}

int sleeping(t_arg *args, int philo_id)
{
	//pthread_mutex_lock(&args->lock[args->nbr_philos]);
	pthread_mutex_lock(&args->lock_printf);
	if (args->exit)
	{
	//	pthread_mutex_unlock(&args->lock[args->nbr_philos]);
		pthread_mutex_unlock(&args->lock_printf);
		return (EXIT);
	}
	gettimeofday(&args->gtime2, NULL);
	printf("%ld %d is sleeping\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 +
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	//pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	pthread_mutex_unlock(&args->lock_printf);
	chad_wastetime(args->timetosleep);
	return (0);
}