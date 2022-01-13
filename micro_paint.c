/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortega- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 15:42:28 by mortega-          #+#    #+#             */
/*   Updated: 2022/01/13 19:30:46 by mortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro_paint.h"

int	n_lines(char *file)
{
	FILE	*fd;
	char	buf;
	int		n;

	n = 0;
	fd = fopen(file, "r");
	while (fread(&buf, 1, 1, fd))
	{
		while (buf == '\n')
			if (!fread(&buf, 1, 1, fd))
				break ;
		if (buf == 'r' || buf == 'R')
			n++;
	}
	fclose(fd);
	return (n);
}

int read_value(char *buf, FILE *fd)
{
	int		value;

	value = 0;
	while (*buf && *buf >= '0' && *buf <= '9')
	{
		value = value * 10 + (*buf - 48);
		fread(buf, 1, 1, fd);
	}
	return (value);
}

float	read_float(char *buf, FILE *fd)
{
	float	numero;
	float	decimal;
	int		n;

	numero = 0;
	decimal = 0;
	n = 0;
	while (*buf && *buf >= '0' && *buf <= '9')
	{
		numero = numero * 10 + (*buf - 48);
		fread(buf, 1, 1, fd);
	}
	if (*buf && *buf == '.')
		fread(buf, 1, 1, fd);
	else if (*buf == ' ')
		return (numero);
	while (*buf && *buf >= '0' && *buf <= '9')
	{
		decimal = decimal * 10 + (*buf - 48);
		n++;
		fread(buf, 1, 1, fd);
	}
	while (n-- > 0)
		decimal = decimal / 10;
	return (numero + decimal);
}

char	jump_spaces(char *buf, FILE *fd)
{
	if (*buf == ' ' || *buf == '\n')
	{
		while (*buf == ' ' || *buf == '\n')
		{
			if (!fread(buf, 1, 1, fd))
				return (2);
		}
		return (0);
	}
	else
		return (1);
}

int	read_first_line(t_mp *fl, FILE *fd)
{
	char	buf;

	fread(&buf, 1, 1, fd);
	fl->width = read_value(&buf, fd);
	if (jump_spaces(&buf, fd))
		return (-1);
	fl->height = read_value(&buf, fd);
	if (jump_spaces(&buf, fd))
		return (-1);
	fl->ch = buf;
	fread(&buf, 1, 1, fd);
	jump_spaces(&buf, fd); 
	return (0);
}

int	read_orders(t_order **or, FILE *fd)
{
	char	buf;
	int		i;

	i = 0;
	while (buf)
	{
		if (buf == 'r' || buf == 'R')
			(*or)[i].orden = buf;
		else
			return (-1);
		fread(&buf, 1, 1, fd);
		jump_spaces(&buf, fd);
		(*or)[i].x = read_float(&buf, fd);
		jump_spaces(&buf, fd);
		(*or)[i].y = read_float(&buf, fd);
		jump_spaces(&buf, fd);
		(*or)[i].w = read_float(&buf, fd);
		jump_spaces(&buf, fd);
		(*or)[i].h = read_float(&buf, fd);
		jump_spaces(&buf, fd);
		(*or)[i].ch = buf;
		fread(&buf, 1, 1, fd);
		if (jump_spaces(&buf, fd) == 2)
			break ;
		i++;
	}
	return (0);
}

int	read_file(char *file, t_mp *fl, t_order **or)
{
	FILE	*fd;

	fl->width = 0;
	fl->height = 0;
	fl->n = n_lines(file);
	*or = (t_order *)malloc(sizeof(t_order) * fl->n);
	fd = fopen(file, "r");
	if (read_first_line(fl, fd))
		return (-1);
	if (read_orders(or, fd))
		return (-1);
	fclose(fd);
	return (0);
}

char	algorithm(t_mp fl, t_order *or, int i, int j)
{
	int		k;
	char	ch;

	ch = fl.ch;
	k = 0;
	while (k < fl.n)
	{
		if (or[k].orden == 'R')
		{
			if (i >= or[k].x && j >= or[k].y)
			{
				if ((i < or[k].x + or[k].w) && (j < or[k].y + or[k].h))
						ch = or[k].ch;
			}
		}
		else if (or[k].orden == 'r')
		{
			if (((i == or[k].x || i == or[k].x + or[k].w) && (j >= or[k].y && j <= or[k].y + or[k].h))
					|| ((j == or[k].y || j == or[k].y + or[k].h) && (i >= or[k].x && i <= or[k].x + or[k].w)))
				ch = or[k].ch;
		}
		k++;
	}
	return (ch);
}

void	paint_figure(t_mp fl, t_order *or)
{
	int i;
	int j;
	int k;
	char	ch;

	i = 1;
	while (i <= fl.width)
	{
		j = 1;
		while (j <= fl.height)
		{
			ch = algorithm(fl, or, i, j);
			write(1, &ch, 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_mp	fl;
	t_order	*or;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (read_file(argv[1], &fl, &or))
	{
		write(1, "Error: File corrupted\n", 32);
		return (1);
	}
	paint_figure(fl, or);
	return (0);
}
