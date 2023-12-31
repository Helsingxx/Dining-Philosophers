/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 22:04:54 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/31 12:01:15 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_finish(t_arg *conv)
{
	pthread_mutex_lock(&conv->lock_finish);
	if (conv->finish)
	{
		pthread_mutex_unlock(&conv->lock_finish);
		return (0);
	}
	pthread_mutex_unlock(&conv->lock_finish);
	return (1);
}

void	*malak_al_mawt(void *arg)
{
	struct timeval	timenow;
	t_arg			*conv;

	conv = (t_arg *)arg;
	pthread_mutex_lock(&conv->lock_readygo);
	pthread_mutex_unlock(&conv->lock_readygo);
	while (1)
	{
		gettimeofday(&timenow, NULL);
		pthread_mutex_lock(&conv->lock_printf);
		if ((conv->timetodie && kill_him(conv,
					((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
						- (conv->gtime.tv_sec * 1000
							+ conv->gtime.tv_usec / 1000))) == FAIL)
			|| (!conv->timetodie
				&& kill_him(conv, 1) == FAIL))
			return (NULL);
		pthread_mutex_unlock(&conv->lock_printf);
		if (check_finish(conv) == 0)
			return (NULL);
	}
	return (NULL);
}

void	*common_routine(void *arg)
{
	int		philo_id;
	t_arg	*conv;
	int		next;

	conv = ((t_arg *)arg)->arg;
	philo_id = ((t_arg *)arg)->philo_id;
	next = philo_id + 1;
	if (philo_id == conv->nbr_philos)
		next = 1;
	pthread_mutex_lock(&conv->lock_readygo);
	pthread_mutex_unlock(&conv->lock_readygo);
	if (conv->ate[philo_id - 1] == conv->to_be_eaten)
		return (NULL);
	if (philo_id % 2 == 0 || philo_id == conv->nbr_philos)
		usleep(100);
	while (1)
	{
		if (eating(conv, philo_id, next) == DONE)
			return (NULL);
		if (sleeping(conv, philo_id) == EXIT)
			return (NULL);
		if (thinking(conv, philo_id) == EXIT)
			return (NULL);
	}
}

void	wait_for_threads(t_arg	*to_free, t_arg *arg, pthread_t *threads,
						char *argv[])
{
	int	x;

	x = 0;
	gettimeofday(&arg->gtime, NULL);
	pthread_mutex_unlock(&arg->lock_readygo);
	while (x < ft_atoi(argv[1]) + 1)
	{
		pthread_join(threads[x++], NULL);
		if (x == ft_atoi(argv[1]))
		{
			pthread_mutex_lock(&arg->lock_finish);
			arg->finish = 1;
			pthread_mutex_unlock(&arg->lock_finish);
		}
	}
	free(to_free);
}

t_arg	*launch_threads(pthread_t *threads, char *argv[], t_arg *arg)
{
	int		cnt;
	t_arg	*args;

	args = ft_calloc(sizeof(t_arg), ft_atoi(argv[1]) + 1);
	cnt = 0;
	pthread_mutex_lock(&arg->lock_readygo);
	while (cnt < ft_atoi(argv[1]) + 1)
	{
		args[cnt].philo_id = cnt + 1;
		args[cnt].arg = arg;
		if (cnt == ft_atoi(argv[1]))
		{
			if (pthread_create(&threads[cnt], NULL, malak_al_mawt, arg))
				return (printf("Creation of a thread has failed!\n"),
					args);
		}
		else if (pthread_create(&threads[cnt], NULL, common_routine,
				&args[cnt]))
			return (printf("Creation of a thread has failed!\n"),
				args);
		cnt++;
	}
	return (args);
}
