/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isint.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 20:37:48 by eamrati           #+#    #+#             */
/*   Updated: 2023/11/28 20:47:22 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

int	ft_pow(int nb, int pow)
{
	if (pow == 0)
		return (1);
	if (pow == 1)
		return (nb);
	return (nb * ft_pow(nb, --pow));
}

int	int_cmp(int int_val, char *nmb, int len, int sign)
{
	int	a;
	int	val;

	a = len;
	if (nmb[0] == '+')
		nmb++;
	while (len)
	{
		val = ((int_val / ft_pow(10, len - 1)) % 10) * sign ;
		if (val < nmb[a - len] - 48)
			return (0);
		else if (val > nmb[a - len] - 48)
			return (1);
		len--;
	}
	return (1);
}

int	int_tst(int max_min)
{
	int	nb;

	nb = 0;
	while (max_min)
	{
		max_min /= 10;
		nb++;
	}
	return (nb);
}

int	max_min_int(char *arg, int len)
{
	int	int_max;
	int	int_min;

	int_max = int_tst(INT_MIN);
	int_min = int_tst(INT_MAX);
	if (arg[0] == '-' || arg[0] == '+')
		len--;
	if ((arg[0] == '-' && int_min < len) || (arg[0] != '-' && int_max < len))
		return (0);
	else if (arg[0] == '-' && int_min == len)
		return (int_cmp(INT_MIN, arg + 1, len, -1));
	else if (arg[0] != '-' && int_max == len)
		return (int_cmp(INT_MAX, arg, len, 1));
	return (1);
}

int	is_int0(char *arg)
{
	int	a;

	a = 0;
	while (*arg == ' ')
		arg++;
	if (arg[a] == '-' || arg[a] == '+')
		a++;
	if (!arg[a] || (arg[a] > 57 || arg[a] < 48))
		return (0);
	while (arg[a])
	{
		if (arg[a] > 57 || arg[a] < 48)
			return (0);
		a++;
	}
	return (max_min_int(arg, a));
}

int	is_int(char *arg)
{
	int	a;

	a = 0;
	while (*arg == ' ')
		arg++;
	if (arg[a] == '-' || arg[a] == '+')
		a++;
	if (!arg[a] || (arg[a] > 57 || arg[a] < 48))
		return (0);
	while (arg[a] && arg[a] != ' ')
	{
		if (arg[a] > 57 || arg[a] < 48)
			return (0);
		a++;
	}
	return (max_min_int(arg, a));
}
