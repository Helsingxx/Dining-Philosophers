/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:01:07 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/20 17:07:23 by eamrati          ###   ########.fr       */
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
	pthread_mutex_t lock_printf;
	pthread_mutex_t lock_readygo;
	pthread_mutex_t *lock;
	pthread_mutex_t lock_finish;
	pthread_mutex_t lock_waiter;
	int				end;
	char			**argv;
	
	int				nbr_philos;
	int				timetodie;
	int				timetoeat;
	int				timetosleep;
	int				gtimestamp;
	int				*times;
	int				*forks;

	int 			nbrtoeat;
	int				*ate;
	int				*time;
	int				exit;
	int				finish;
	int				to_be_eaten;
	
	struct timeval gtime;
	struct timeval gtime2;
	struct s_arg 	*arg;
} t_arg;

int	is_int(char *);
int	ft_atoi(const char *);
int	skip_int(char *);
void check_elapsed(int, int, t_arg *);
int 	eating(t_arg *, int, int);
int		thinking(t_arg *, int);
int		sleeping(t_arg *, int);
void	*ft_calloc(size_t, size_t);
int kill_him(t_arg *, int);
int	is_int0(char *);
int	ft_strlen(const char *);
void chad_wastetime(int);

#endif  