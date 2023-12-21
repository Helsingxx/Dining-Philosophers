/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eamrati <eamrati@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 21:23:44 by eamrati           #+#    #+#             */
/*   Updated: 2023/12/21 17:46:11 by eamrati          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_strlen(const char *s)
{
	int	a;

	a = 0;
	if (!s)
		return (0);
	while (s[a] != '\x00')
		a++;
	return (a);
}

long	ft_nbcmp(const char *s1, const char *s2)
{
	int	a;
	int	b;

	b = 0;
	a = 0;
	while (s1[b] == '0')
		b++;
	s1 = &s1[b];
	b = 0;
	while (s1[b] >= 48 && s1[b] <= 57)
		b++;
	if (b > ft_strlen(s2))
		return (1);
	else if (b < ft_strlen(s2))
		return (-1);
	while (s2[a] != 0 && s1[a] >= 48 && s1[a] <= 57)
	{
		if (s1[a] > s2[a])
			return (1);
		else if (s1[a] < s2[a])
			return (-1);
		a++;
	}
	return (0);
}

static unsigned long	positive(const char *str, int a, int sign)
{
	unsigned long	number;

	number = 0;
	if (sign == 1 && ft_nbcmp(&str[a], "9223372036854775807") > 0)
		return (-1);
	if (sign == -1 && ft_nbcmp(&str[a], "9223372036854775808") > 0)
		return (0);
	while (str[a] >= 48 && str[a] <= 57)
	{
		number *= 10;
		number += str[a] - 48;
		a++;
	}
	return (number);
}

int	ft_atoi(const char *str)
{
	int	a;

	a = 0;
	while (str[a] == '\t' || str[a] == '\n' || str[a] == '\v'
		|| str[a] == '\f' || str[a] == '\r' || str[a] == ' ')
		a++;
	if (str[a] == '-')
	{
		a++;
		return (positive(str, a, -1) * -1);
	}
	else if (str[a] == '+')
		a++;
	return (positive(str, a, 1));
}
