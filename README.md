<h1> DESCRIPTION
```
This project runs a valid simulation of the popular "Dining Philosophers Problem"
using either mutexes (main) or semphores (bonus).
The second solution uses semaphores, however it is based on a different
iteration of the problem where the philosophers are queued to use forks
placed at the center of the table. It also adds the requirement to
replace the use of threads with that of processes.
The problem was solved with heavy constraints on the use of system functions.
```
<h1> NOTICE
```
Do not run the simulation with excessively high or low positive values,
otherwise the simulation might fail depending on the hardware it is ran.
```
<h1> USAGE
```
$ cd philo(_bonus)
$ make
$ ./philo(_bonus) [Number of philosophers] [Time to die] [Time to eat] [Time to sleep]
```
