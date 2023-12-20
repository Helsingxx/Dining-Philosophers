/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:33 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/20 16:59:07 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void  *malak_al_mawt(void *arg)
{
	struct timeval timenow;
	int				times;
	int				inc;
	t_arg			*conv;

	conv = (t_arg *)arg;
	times = 0;
	inc = 0;
	pthread_mutex_lock(&conv->lock_readygo);
	pthread_mutex_unlock(&conv->lock_readygo);
	while (1)
	{
	//	pthread_mutex_lock(&conv->lock[conv->nbr_philos]);
		gettimeofday(&timenow, NULL);
		printf("check time %ld\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)));
		pthread_mutex_lock(&conv->lock_printf);
		gettimeofday(&timenow, NULL);
		printf("PERM time %ld\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)));
		if (kill_him(conv, (conv->timetodie && (((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) / conv->timetodie)),
				((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000))) == FAIL)
			return (NULL);
		gettimeofday(&timenow, NULL);
		printf("Before nothing here %ld\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)));
		pthread_mutex_lock(&conv->lock_finish);
		if (conv->finish)
		{
			pthread_mutex_unlock(&conv->lock_finish);
			return (NULL);
		}
		pthread_mutex_unlock(&conv->lock_finish);
		pthread_mutex_unlock(&conv->lock_printf);
	//	pthread_mutex_unlock(&conv->lock[conv->nbr_philos]);
		gettimeofday(&timenow, NULL);
		printf("Finish time %ld\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)));
	}
	return (NULL);
}

void *common_routine(void *arg)
{
	int philo_id;
	t_arg *conv;
	int next;

	conv = ((t_arg *)arg)->arg;
	philo_id = ((t_arg *)arg)->philo_id;
	next = philo_id + 1;
	if (philo_id == conv->nbr_philos)
		next = 1;
	pthread_mutex_lock(&conv->lock_readygo);
	pthread_mutex_unlock(&conv->lock_readygo);
	if (conv->ate[philo_id - 1] == conv->to_be_eaten)
		return (NULL);
	if (philo_id % 2 == 0)
		usleep(100); // pay attention
	while (1)
	{
		usleep(1 * philo_id);
		if (eating(conv, philo_id, next) == DONE)
			return (NULL);
		usleep(1 * philo_id);
		if (sleeping(conv, philo_id) == EXIT)
			return (NULL);
		usleep(1 * philo_id);
		if (thinking(conv, philo_id) == EXIT)
			return (NULL);
	}
}

void __freeall(pthread_t *threads, t_arg *arg)
{
	int x;

	x = 0;
	free(arg->ate);
	free(arg->lock);
	free(threads);
}

void	*ft_calloc(size_t count, size_t size)
{
	unsigned long long		a;
	char					*ptr;

	a = 0;
	ptr = malloc(count * size);
	if (ptr == 0)
		return (0);
	while (a < count * size)
	{
		ptr[a++] = 0;
	}
	return (ptr);
}

int parse(int argc, char *argv[])
{
	int scount;
	int eatcnt;

	eatcnt = 0;
	scount = 1;
	if (argc < 5 || argc > 6)
		return (printf("Wrong number of args\n"), 1);
	while (scount < argc)
		if (!is_int0(argv[scount]) || ft_atoi(argv[scount++]) < 0) // test this later
			return (printf("Incorrect arguments\n"), 1);
	return (SUCCESS);
}

void __fargs(t_arg **args, int cnt)
{
	int x;

	x = 0;
	while (x < cnt)
		free(args[x++]);
	free(args);
} // to verify

//int max_arr(int *arr, int len)
//{
//	int x;
//	int max;
//	
//	x = 0;
//	max = arr[0];
//	while (x < len)
//	{
//		if (max < arr[x])
//			max = arr[x];
//		x++;
//	}
//	return (max);
//}

int init_arg(t_arg *arg, char **argv)
{
	arg->argv = argv;
	arg->nbr_philos = ft_atoi(argv[1]);
	arg->timetodie = ft_atoi(argv[2]);
	arg->timetoeat = ft_atoi(argv[3]);
	arg->timetosleep = ft_atoi(argv[4]);
	if (argv[5])
		arg->to_be_eaten = ft_atoi(argv[5]);
	else
		arg->to_be_eaten = -1;
	arg->ate = (int *) ft_calloc(sizeof(int), arg->nbr_philos);
	if (!arg->ate)
		return (FAIL);
	arg->lock = (pthread_mutex_t *) ft_calloc(sizeof(pthread_mutex_t), arg->nbr_philos + 1);
	if (!arg->lock)
		return (free(arg->ate), FAIL);
	return (SUCCESS);
}

int launch_threads(pthread_t *threads, char *argv[], t_arg *arg)
{
	int cnt;
	t_arg *args;

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
				cnt);
		}
		else if (pthread_create(&threads[cnt], NULL, common_routine, &args[cnt]))
			return (printf("Creation of a thread has failed!\n"),
				//__freeall(arg->nbrtoeat, threads, ft_atoi(argv[1])),
				//__fargs(args, cnt),
					cnt);
		cnt++;
	}
	return (cnt);
}

void wait_for_threads(int cnt, t_arg *arg, pthread_t *threads)
{
	int x;

	x = 0;
	gettimeofday(&arg->gtime, NULL);
	pthread_mutex_unlock(&arg->lock_readygo);
	while (x < cnt)
	{
		pthread_join(threads[x++], NULL);
		if (x == cnt - 1)
		{
			pthread_mutex_lock(&arg->lock_finish);
			arg->finish = 1;
			pthread_mutex_unlock(&arg->lock_finish);
		}
	}
}

void __fmutexes(pthread_mutex_t *mutarr, int x)
{
	int c;

	c = 0;
	while (c < x)
		pthread_mutex_destroy(&mutarr[c++]);
}

int init_mutexes(t_arg* arg)
{
	int x;

	x = 0;
	if (pthread_mutex_init(&arg->lock_readygo, NULL))
		return (FAIL);
	if (pthread_mutex_init(&arg->lock_printf, NULL))
		return (pthread_mutex_destroy(&arg->lock_readygo),
				FAIL);
	if (pthread_mutex_init(&arg->lock_finish, NULL))
		return (pthread_mutex_destroy(&arg->lock_readygo),
				pthread_mutex_destroy(&arg->lock_printf),
				FAIL);
	while (x < arg->nbr_philos + 1)
		if (pthread_mutex_init(&arg->lock[x++], NULL))
			return (__fmutexes(arg->lock, x), 
				pthread_mutex_destroy(&arg->lock_readygo),
				pthread_mutex_destroy(&arg->lock_printf),
				pthread_mutex_destroy(&arg->lock_finish),
				FAIL);
	return (SUCCESS);
}

int main(int argc, char *argv[])
{
	t_arg *arg;
	pthread_t *threads;
	
	arg = (t_arg *)ft_calloc(sizeof(t_arg), 1);
	if (parse(argc, argv))
		return (FAIL);
	if (!ft_atoi(argv[1]))
		return (printf("Null simulation"), FAIL);
	threads = (pthread_t *) ft_calloc(sizeof(pthread_t), ft_atoi(argv[1]) + 1 + 3);
	if (!threads)
		return (FAIL);
	if (init_arg(arg, argv) == FAIL)
		return (free(threads), FAIL);
	if (init_mutexes(arg) == FAIL)
		return (__freeall(threads, arg), FAIL); // Check fail
	wait_for_threads(launch_threads(threads, argv, arg), arg, threads);
	return (ft_atoi(argv[1]),
			__fmutexes(arg->lock, arg->nbr_philos),
			pthread_mutex_destroy(&arg->lock_readygo),
			pthread_mutex_destroy(&arg->lock_printf),
			pthread_mutex_destroy(&arg->lock_finish),
			__freeall(threads, arg),
			SUCCESS);
}