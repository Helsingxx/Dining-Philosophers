/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   takeforksandsomegarbage.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 22:07:07 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:09:01 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_first_fork(t_arg *args, int philo_id)
{
	pthread_mutex_lock(&(args->lock[philo_id - 1]));
	gettimeofday(&args->gtime2, NULL);
	pthread_mutex_lock(&args->lock_printf);
	if (!args->exit)
		printf("%ld %d has taken a fork\n", (args->gtime2.tv_sec * 1000
				+ args->gtime2.tv_usec / 1000)
			- (args->gtime.tv_sec * 1000 + args->gtime.tv_usec / 1000),
			philo_id);
	pthread_mutex_unlock(&args->lock_printf);
}

void	try_take_scnd(t_arg *args, int philo_id, int next)
{
	if (args->nbr_philos != 1)
		pthread_mutex_lock(&args->lock[next - 1]);
	else
		chad_wastetime(args->timetodie);
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (!args->exit)
		printf("%ld %d has taken a fork\n", (args->gtime2.tv_sec * 1000
				+ args->gtime2.tv_usec / 1000)
			- (args->gtime.tv_sec * 1000
				+ args->gtime.tv_usec / 1000), philo_id);
	pthread_mutex_unlock(&args->lock_printf);
}

void	__fmutexes(pthread_mutex_t *mutarr, int x)
{
	int	c;

	c = 0;
	while (c < x - 1)
		pthread_mutex_destroy(&mutarr[c++]);
}

void	__freeall(pthread_t *threads, t_arg *arg)
{
	int	x;

	x = 0;
	free(arg->ate);
	free(arg->lock);
	free(arg->lasttimeate);
	free(arg);
	free(threads);
}
