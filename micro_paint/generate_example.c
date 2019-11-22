/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_example.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:09:44 by ncolomer          #+#    #+#             */
/*   Updated: 2019/11/22 19:17:56 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

# define FULL_ASCII 0
# define MODE "rR" // -> "cC"

int
	rand_int_range(int min, int max)
{
	return (rand() % (max + 1 - min) + min);
}

float
	rand_float_range(float min, float max)
{
    return (min + (rand() / (float)(RAND_MAX / (float)(max - min))));
}

int
	main(void)
{
	FILE*	file;
	int		width, height, background;
	int		i, nbr_shapes;
	int		type, color;
	double	x, y, swidth, sheight;
	int		size;

	srand(time(NULL));
	if (!(file = fopen("example_", "w")))
		return (printf("file: fopen error.\n"));
	width = rand_int_range(-1, 301);
	height = rand_int_range(-1, 301);
	background = rand_int_range((FULL_ASCII) ? 0 : 32, (FULL_ASCII) ? 255 : 126);
	if (fprintf(file, "%d %d %c\n", width, height, background) < 0)
		return (fclose(file) && printf("file: fprintf error.\n"));

	nbr_shapes = rand_int_range(-4, 42);
	i = 0;
	while (i < nbr_shapes)
	{
		type = rand_int_range(0, 10000);
		if (type == 10000 || type == 0)
			type = 'a';
		else if (type >= 5000)
			type = 'r';
		else
			type = 'R';
		color = rand_int_range((FULL_ASCII) ? 0 : 32, (FULL_ASCII) ? 255 : 126);
		if ((size = rand_int_range(0, 100)) >= 50)
		{
			x = rand_float_range(.0001, (double)size);
			y = rand_float_range(.0001, (double)size);
			swidth = rand_float_range(.0001, (double)size);
			sheight = rand_float_range(.0001, (double)size);
		}
		else
		{
			x = rand_float_range(-100., 400.);
			y = rand_float_range(-100., 400.);
			swidth = rand_float_range(-.01, 400.);
			sheight = rand_float_range(-.01, 400.);
		}
		if (rand_int_range(0, 100) >= 35)
			size = fprintf(file, (++i == nbr_shapes) ? "%c %f %f %f %f %c" : "%c %f %f %f %f %c\n", type, x, y, swidth, sheight, color);
		else
			size = fprintf(file, (++i == nbr_shapes) ? "%c %d %d %d %d %c" : "%c %d %d %d %d %c\n", type, (int)x, (int)y, (int)swidth, (int)sheight, color);
		if (size < 0)
			return (fclose(file) && printf("file: fprintf error.\n"));
	}
	fclose(file);
	return (0);
}
