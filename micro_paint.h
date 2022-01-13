/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortega- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/23 15:39:18 by mortega-          #+#    #+#             */
/*   Updated: 2022/01/13 19:28:27 by mortega-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICRO_PAINT_H
# define MICRO_PAINT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_mp
{
	int		width;
	int		height;
	char	ch;
	int		n;
}			t_mp;

typedef struct s_order
{
	char	orden;
	float	x;
	float	y;
	float	w;
	float	h;
	char	ch;
}			t_order;

#endif
