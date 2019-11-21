/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 12:11:01 by ncolomer          #+#    #+#             */
/*   Updated: 2019/11/21 17:48:16 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	background;
}				t_zone;

typedef struct	s_shape
{
	char			type;
	double			x;
	double			y;
	double			width;
	double			height;
	char			color;
	struct s_shape	*next;
}				t_shape;

int
	ft_strlen(char const *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

t_shape
	*new_shape(void)
{
	t_shape	*tmp;

	if (!(tmp = (t_shape*)malloc(sizeof(*tmp))))
		return (NULL);
	tmp->type = 0;
	tmp->x = 0;
	tmp->y = 0;
	tmp->width = 0;
	tmp->height = 0;
	tmp->color = 0;
	tmp->next = NULL;
	return (tmp);
}

int
	check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300
			&& zone->height > 0 && zone->height <= 300
			&& zone->background != 0);
}

int
	check_shape(t_shape *s)
{
	return (s->width > 0 && s->height > 0 && s->color != 0
			&& (s->type == 'r' || s->type == 'R'));
}

int
	parse_file(FILE *file, t_zone *zone, t_shape **shape)
{
	t_shape	*first;
	t_shape	*t;

	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background) != 3
		|| !check_zone(zone))
		return (0);
	if (!(t = new_shape()))
		return (0);
	first = t;
	while (fscanf(file, "%c %lf %lf %lf %lf %c\n", &t->type, &t->x, &t->y, &t->width, &t->height, &t->color) == 6)
	{
		if (!*shape)
		{
			first = t;
			*shape = t;
		}
		else
		{
			(*shape)->next = t;
			*shape = t;
		}
		if (!check_shape(t)
			|| !(t = new_shape()))
		{
			*shape = first;
			return (0);
		}
	}
	*shape = first;
	return (1);
}

void
	draw_shape(char *drawing, t_zone *zone, t_shape *shape)
{
	int	i;
	int	j;
	int	lim_x;
	int	lim_y;

	if (shape->type == 'r')
	{
		lim_y = floor(shape->y + shape->height);
		i = ceil(shape->y);
		while (i <= lim_y)
		{
			if (i >= 0 && i < zone->height)
			{
				lim_x = floor(shape->x + shape->width);
				j = ceil(shape->x);
				while (j <= lim_x)
				{
					if (j >= 0 && j < zone->width)
					{
						if (i == ceil(shape->y)
							|| i == lim_y
							|| j == ceil(shape->x)
							|| j == lim_x)
							drawing[(i * zone->width) + j] = shape->color;
					}
					j++;
				}
			}
			i++;
		}
	}
	else
	{
		lim_y = floor(shape->y + shape->height);
		i = ceil(shape->y);
		while (i <= lim_y)
		{
			if (i >= 0 && i < zone->height)
			{
				lim_x = floor(shape->x + shape->width);
				j = ceil(shape->x);
				while (j <= lim_x)
				{
					if (j >= 0 && j < zone->width)
						drawing[(i * zone->width) + j] = shape->color;
					j++;
				}
			}
			i++;
		}
	}
}

char
	*new_drawing(t_zone *zone)
{
	char	*drawing;
	int		i;
	int		j;

	if (!(drawing = (char*)malloc(sizeof(*drawing) * (zone->width * zone->height))))
		return (NULL);
	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
			drawing[(i * zone->width) + j++] = zone->background;
		i++;
	}
	return (drawing);
}

void
	print_drawing(char *drawing, t_zone *zone)
{
	int	i;
	int	j;

	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
			write(1, drawing + (i * zone->width) + j++, 1);
		write(1, "\n", 1);
		i++;
	}
}

int
	clear_all(FILE* file, t_shape **shapes, char *drawing)
{
	t_shape	*next;

	fclose(file);
	while (*shapes)
	{
		next = (*shapes)->next;
		free(*shapes);
		*shapes = next;
	}
	if (drawing)
		free(drawing);
	return (1);
}

int
	output_error(char const *err, int ret)
{
	write(1, err, ft_strlen(err));
	return (ret);
}

int
	main(int argc, char **argv)
{
	char	*drawing;
	t_zone	zone;
	t_shape	*shapes;
	FILE	*file;
	t_shape	*next;

	if (argc != 2)
		return (output_error("Error: argument\n", 1));
	zone.width = -1;
	zone.height = -1;
	zone.background = 0;
	shapes = NULL;
	drawing = NULL;
	if (!(file = fopen(argv[1], "r")))
		return (output_error("Error: Operation file corrupted\n", 1));
	if (!(parse_file(file, &zone, &shapes)))
		return (clear_all(file, &shapes, drawing) && output_error("Error: Operation file corrupted\n", 1));
	if (!(drawing = new_drawing(&zone)))
		return (clear_all(file, &shapes, drawing) && output_error("Error: malloc failed :)\n", 1));
	while (shapes)
	{
		draw_shape(drawing, &zone, shapes);
		next = shapes->next;
		free(shapes);
		shapes = next;
	}
	print_drawing(drawing, &zone);
	clear_all(file, &shapes, drawing);
	return (0);

	/*

	printf("%d %d %c\n", zone.width, zone.height, zone.background);
	t_shape *first;
	first = shapes;
	while (shapes)
	{
		printf("%c %lf %lf %lf %lf %c\n", shapes->type, shapes->x, shapes->y, shapes->width, shapes->height, shapes->color);
		shapes = shapes->next;
	}
	shapes = first;
	clear_all(file, &shapes);

	*/
}
