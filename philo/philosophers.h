/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:01:07 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/26 22:09:35 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# define FAIL 255
# define SUCCESS 0
# define DONE 50
# define LAP 1
# define EXIT 50
# define ALLOW 1
# define DENY 0

typedef struct s_arg
{
	int				philo_id;
	pthread_mutex_t	lock_printf;
	pthread_mutex_t	lock_readygo;
	pthread_mutex_t	*lock;
	pthread_mutex_t	lock_finish;
	pthread_mutex_t	lock_waiter;
	int				end;
	char			**argv;

	int				nbr_philos;
	int				timetodie;
	int				timetoeat;
	int				timetosleep;
	int				gtimestamp;
	int				*times;
	int				*forks;

	int				nbrtoeat;
	int				*ate;
	int				*time;
	int				exit;
	int				finish;
	int				to_be_eaten;
	int				*eating_updates;

	time_t			*lasttimeate;
	struct timeval	gtime;
	struct timeval	gtime2;
	struct s_arg	*arg;
}	t_arg;

int		is_int0(char *arg);
int		ft_atoi(const char *str);
int		skip_int(char *arg);
int		eating(t_arg *args, int philo_id, int next);
int		thinking(t_arg *args, int philo_id);
int		sleeping(t_arg *args, int philo_id);
void	*ft_calloc(size_t count, size_t size);
int		kill_him(t_arg *arg, int times);
int		ft_strlen(const char *s);
void	chad_wastetime(int milliseconds);
void	try_take_scnd(t_arg *args, int philo_id, int next);
void	take_first_fork(t_arg *args, int philo_id);
void	__fmutexes(pthread_mutex_t *mutarr, int x);
void	__freeall(pthread_t *threads, t_arg *arg);
int		check_finish(t_arg *conv);
void	*malak_al_mawt(void *arg);
void	*common_routine(void *arg);
void	wait_for_threads(t_arg	*to_free, t_arg *arg, pthread_t *threads,
			char *argv[]);
t_arg	*launch_threads(pthread_t *threads, char *argv[], t_arg *arg);

#endif