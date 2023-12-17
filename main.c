/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 11:42:33 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/16 23:36:27 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void  *malak_al_mawt(void *arg)
{
	struct timeval timenow;
	int				times;
	int				x;
	t_arg			*conv;

	conv = (t_arg *)arg;
	x = 0;
	times = 0;
	while (times != conv->to_be_eaten)
	{
		gettimeofday(&timenow, NULL);
		while (((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
			- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) % conv->timetodie != 0)
			gettimeofday(&timenow, NULL);
		pthread_mutex_lock(&conv->lock[conv->nbr_philos]);
		while (x < conv->nbr_philos)
		{
			if (kill_him(conv, timenow, times, x) == FAIL)
				return (NULL);
			x++;
		}
		times++;
		pthread_mutex_unlock(&conv->lock[conv->nbr_philos]);
	}
	return (NULL);
}

void *common_routine(void *arg)
{
	int	loc_nte;
	int philo_id;
	t_arg *conv;
	int next;

	conv = ((t_arg *)arg)->arg;
	philo_id = ((t_arg *)arg)->philo_id;
	next = philo_id + 1;
	if (philo_id == conv->nbr_philos)
		next = 1;
	loc_nte = conv->nbrtoeat;
	pthread_mutex_lock(&conv->lock_rg);
	pthread_mutex_unlock(&conv->lock_rg);
	if (!(philo_id % 2))
		usleep(5);
	while (1)
	{
		if (eating(conv, philo_id, next, &loc_nte) == DONE)
			return (NULL);
		sleeping(conv, philo_id);
		if (thinking(conv, philo_id) == EXIT)
			return (NULL);
	}
}

void __freeall(pthread_t *threads, int end)
{
	int x;

	x = 0;
	//while (x < end)
	//	free(threads[x++]);
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

int	*philosnbrtoeat(char *argv[], int *numbers)
{
	int b;
	int cb;

	b = 0;
	cb = 0;
	while (argv[5][b] && numbers)
	{
		numbers[cb] = ft_atoi(&argv[5][b]);
		b += skip_int(&argv[5][b]);
		cb++;
	}
	return (numbers);
}

pthread_t *init_threadarr(char *argv[])
{
	pthread_t *threads;
	int			x;

	x = 0;
	threads = (pthread_t *) ft_calloc(sizeof(pthread_t), ft_atoi(argv[1]) + 1);
	if (!threads)
		return ( NULL);
/*	while (x < ft_atoi(argv[1]))
	{
		threads[x] = (pthread_t *) ft_calloc (1, sizeof(pthread_t));
		if (!threads[x])
			return (__freeall(nbrtoeat, threads, x), NULL);
		x++;
	}
*/
	return (threads);
}

int *parse(int argc, char *argv[])
{
	int scount;
	int eatcnt;

	eatcnt = 0;
	scount = 1;
	if (argc < 5 || argc > 6)
		return (printf("Wrong number of args\n"), NULL);
	while (scount < argc)
		if (!is_int0(argv[scount]) || ft_atoi(argv[scount++]) < 0) // test this later
			return (printf("Incorrect arguments\n"), NULL);
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
	arg->exit = 0;
	arg->to_be_eaten = ft_atoi(argv[5]);
	arg->ate = (int *) ft_calloc(sizeof(int), arg->nbr_philos);
	if (!arg->ate)
		return (FAIL);
	arg->lock = (pthread_mutex_t *) ft_calloc(sizeof(pthread_mutex_t), arg->nbr_philos + 1);
	if (!arg->lock)
		return (free(arg->ate), FAIL);
	return (SUCCESS);
}

#include <errno.h>

int launch_threads(pthread_t *threads, char *argv[], t_arg *arg, t_arg **fill)
{
	int cnt;
	t_arg *args;

	args = ft_calloc(sizeof(t_arg), ft_atoi(argv[1]) + 1);
	*fill = args;
	cnt = 0;
	pthread_mutex_lock(&arg->lock_rg);
	while (cnt < ft_atoi(argv[1]) + 1)
	{
		args[cnt].philo_id = cnt + 1;
		args[cnt].arg = arg;
		if (cnt == ft_atoi(argv[1]))
			if (pthread_create(&threads[cnt], NULL, malak_al_mawt, &args[cnt]))
				return (printf("Creation of a thread has failed!\n"),
				cnt);
		if (pthread_create(&threads[cnt], NULL, common_routine, &args[cnt]))
			return (printf("Creation of a thread has failed!\n"),
				//__freeall(arg->nbrtoeat, threads, ft_atoi(argv[1] + 1)),
				//__fargs(args, cnt),
				cnt);
		cnt++;
	}// destroy simulation if fail!
	return (cnt);
}

void wait_for_threads(int cnt, t_arg *args, pthread_t *threads, char **argv)
{
	int x;
	int death;
	t_arg *arg;
	int nbr_philos;

	nbr_philos = 0;
	death = 0;
	x = 0;
	arg = args[0].arg;
	arg->time = ft_calloc(sizeof(int), arg->nbr_philos); // if fail
	gettimeofday(&arg->gtime, NULL);
	pthread_mutex_unlock(&arg->lock_rg);
	while (x < cnt)
		pthread_join(threads[x++], NULL);
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
	int loc_rg;

	x = 0;
	if (pthread_mutex_init(&arg->lock_rg, NULL))
		return (FAIL);
	while (x < arg->nbr_philos + 1)
		if (pthread_mutex_init(&arg->lock[x++], NULL))
			return (__fmutexes(arg->lock, x), 
				pthread_mutex_destroy(&arg->lock_rg),
				FAIL);
	return (SUCCESS);
}

int main(int argc, char *argv[])
{
	t_arg *arg;
	pthread_t *threads;
	t_arg *args;
	
	arg = (t_arg *)ft_calloc(sizeof(t_arg), 1);
	if (parse(argc, argv))
		return (FAIL);
	if (!ft_atoi(argv[1]))
		return (printf("Null simulation"), FAIL);
	threads = init_threadarr(argv);
	if (!threads)
		return (FAIL);
	init_arg(arg, argv);
	init_mutexes(arg); // Check fail
	wait_for_threads(launch_threads(threads, argv, arg, &args), args, threads, argv);
	return (__freeall(threads, ft_atoi(argv[1])),
			pthread_mutex_destroy(arg->lock), // This should be _fmutexes
			SUCCESS);
}