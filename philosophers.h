/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 15:01:07 by eamrati           #+#    #+#             */
/*   Updated: 2023/11/29 18:32:32 by eamrati          ###   ########.fr       */
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
# define EXIT 666

typedef struct s_arg
{	
	int				philo_id;
	pthread_mutex_t lock_printf;
	pthread_mutex_t lock_rg;
	pthread_mutex_t *lock;
	int				end;
	char			**argv;
	
	int				nbr_philos;
	int				timetodie;
	int				timetoeat;
	int				timetosleep;
	int				gtimestamp;
	int				*times;

	int 			*nbrtoeat;
	int				*ate;
	int				*time;
	int				exit;
	int				to_be_eaten;

	struct timeval gtime;
	struct timeval gtime2;
	struct s_arg 	*arg;
} t_arg;

int	is_int(char *);
int	ft_atoi(const char *);
int	skip_int(char *);
void check_elapsed(int, int, t_arg *);
int 	eating(t_arg *, int, int , int *);
int		thinking(t_arg *, int);
void 	sleeping(t_arg *, int);
void	*ft_calloc(size_t, size_t);
int kill_him(t_arg *, struct timeval, int, int);
int	is_int0(char *);
size_t	ft_strlen(const char *);
void chad_wastetime(int);

#endif