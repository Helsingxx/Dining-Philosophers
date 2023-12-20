void *waiter(void *arg)
{
	t_arg			*conv;
	int				phil_id;

	conv = (t_arg *)arg;
	while (1)
	{
		phil_id = 0;
		while (phil_id < conv->nbr_philos)
		{
			pthread_mutex_lock(&conv->lock_waiter);
			if (!((phil_id + 1) % 2) && conv->forks[phil_id - 1])// || (phil_id == 0 && conv->forks[conv->nbr_philos - 1] == 9841)) //handle odd number of philosophers by making first philo odd and even at the same time
			{
				conv->forks[phil_id] = DENY;
			}
			else if (!((phil_id + 1) % 2))
			{
				conv->forks[phil_id] = ALLOW;
			//	printf("at least\n");
			}
			pthread_mutex_unlock(&conv->lock_waiter);
			phil_id++;
		}
	}
}

void dbgtime(t_arg *arg)
{
	struct timeval timenow;
	gettimeofday(&timenow, NULL);
	printf("passed %ld\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (arg->gtime.tv_sec * 1000 + arg->gtime.tv_usec / 1000)));
}

	//pthread_mutex_lock(&args->lock_waiter);
	//if ((philo_id % 2 == 0 || philo_id == args->nbr_philos) && args->forks[philo_id - 1] == DENY)
	//{
	//	while (args->forks[philo_id - 1] == DENY)
	//		(pthread_mutex_unlock(&args->lock_waiter),
	//		pthread_mutex_lock(&args->lock_waiter));
	//}
	//pthread_mutex_unlock(&args->lock_waiter);

		//if (philo_id % 2 && philo_id != args->nbr_philos)
	//	(pthread_mutex_lock(&args->lock_waiter), args->forks[philo_id] = 1, pthread_mutex_unlock(&args->lock_waiter));
		//if (philo_id % 2 && philo_id != args->nbr_philos)
	//	(pthread_mutex_lock(&args->lock_waiter), args->forks[philo_id] = 0, pthread_mutex_unlock(&args->lock_waiter));



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
	while (++inc != conv->to_be_eaten)
	{	
		gettimeofday(&timenow, NULL);
		if (conv->timetodie && ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) / conv->timetodie > inc)
			inc = ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
				- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) / conv->timetodie;
		while (1)//conv->timetodie && (((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
			//- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) / conv->timetodie))// < inc))
		{
			gettimeofday(&timenow, NULL);
			pthread_mutex_lock(&conv->lock[conv->nbr_philos]);
			if (kill_him(conv, timenow, (conv->timetodie && (((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
					- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) / conv->timetodie))) == FAIL)
				return (NULL);
			pthread_mutex_unlock(&conv->lock[conv->nbr_philos]);
		}
		if (conv->timetodie)
			inc = ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
			- (conv->gtime.tv_sec * 1000 + conv->gtime.tv_usec / 1000)) / conv->timetodie;

		pthread_mutex_lock(&conv->lock_finish);
		if (conv->finish)
			return (NULL);
		pthread_mutex_unlock(&conv->lock_finish);
	}
	return (NULL);
}s