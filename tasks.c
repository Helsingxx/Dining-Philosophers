/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:29 by eamrati           #+#    #+#             */
/*   Updated: 2023/11/30 10:30:45 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


int kill_him(t_arg *arg, struct timeval timenow, int times, int x)
{
	if (arg->ate[x] != times && arg->ate[x] != arg->nbrtoeat[x])
	{
		pthread_mutex_lock(&arg->lock_printf);
		printf("%ld %d has died\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (arg->gtime.tv_sec * 1000 + arg->gtime.tv_usec / 1000)), x + 1);
		arg->exit = 1;
		pthread_mutex_unlock(&arg->lock_printf);
		pthread_mutex_unlock(&arg->lock[arg->nbr_philos]);
		return (FAIL); // does not unlock. prevents segfault
	}
	return (LAP);
}

int eating(t_arg *args, int philo_id, int next, int *nte)
{
	int forks;

	forks = 0;
	if (forks <= args->nbr_philos)
		forks++;
	pthread_mutex_lock(&(args->lock[philo_id - 1]));
	if (forks < args->nbr_philos)
		forks++;
	else
		chad_wastetime(args->timetodie);
	pthread_mutex_lock(&args->lock[next - 1]);

	pthread_mutex_lock(&args->lock[args->nbr_philos]);
	args->ate[philo_id - 1]++ ;
	gettimeofday(&args->gtime2, NULL);
	if (!args->exit)
		printf("%ld %d is eating\n",(args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id); // picked up forks!!! add the message
	pthread_mutex_unlock(&args->lock[args->nbr_philos]);

	chad_wastetime(args->timetoeat);

	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (!args->exit)
		printf("%ld %d is done eating\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	pthread_mutex_unlock(&args->lock_printf);

	pthread_mutex_unlock(&args->lock[next - 1]);
	pthread_mutex_unlock(&args->lock[philo_id - 1]);
	(*nte)--;
	if (!*nte)
		return (DONE); // SUCCESS!
	return (LAP);
}

int thinking(t_arg *args, int philo_id)
{	
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (!args->exit)
		printf("%ld %d is thinking\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	pthread_mutex_lock(&args->lock[args->nbr_philos]);
	if (args->exit)
	{
		return (EXIT);
		pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	}
	pthread_mutex_unlock(&args->lock[args->nbr_philos]);
	return (LAP);
}

void sleeping(t_arg *args, int philo_id)
{
	pthread_mutex_lock(&args->lock_printf);
	gettimeofday(&args->gtime2, NULL);
	if (!args->exit)
		printf("%ld %d is sleeping\n", (args->gtime2.tv_sec - args->gtime.tv_sec) * 1000 + 
									(args->gtime2.tv_usec - args->gtime.tv_usec) / 1000, philo_id);
	pthread_mutex_unlock(&args->lock_printf);
	chad_wastetime(args->timetosleep);
}