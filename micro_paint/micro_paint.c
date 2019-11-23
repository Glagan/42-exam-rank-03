#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_zone
{
	int		width;
	int		height;
	char	background;
} 				t_zone;

typedef struct	s_shape
{
	char	type;
	double	x;
	double	y;
	double	width;
	double	height;
	char	color;
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

int
	ft_floor(double val)
{
	return ((int)val);
}

int
	ft_ceil(double val)
{
	int	att;

	att = (int)val;
	if (val - att > 0)
		return ((int)((double)1. + (double)val));
	return (att);
}

int
	str_error(char const *str, int ret)
{
	write(1, str, ft_strlen(str));
	return (ret);
}

int
	clear_all(FILE *file)
{
	fclose(file);
	return (1);
}

int
	check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300
			&& zone->height > 0 && zone->height <= 300);
}

int
	check_shape(t_shape *shape)
{
	return (shape->width > 0 && shape->height > 0
			&& (shape->type == 'r' || shape->type == 'R')
			/*&& shape->color != ' '*/);
}

int
	get_zone(FILE *file, t_zone *zone)
{
	int scan_ret;

	if ((scan_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (!check_zone(zone))
		return (0);
	if (scan_ret == -1)
		return (0);
	return (1);
}

void
	paint_background(char drawing[900], t_zone *zone)
{
	int	i;

	i = 0;
	while (i < 900)
		drawing[i++] = zone->background;
}

int
	draw_shapes(FILE *file, char drawing[900], t_zone *zone)
{
	t_shape	tmp;
	int		scan_ret;

	while ((scan_ret = fscanf(file, "%c %lf %lf %lf %lf %c\n", &tmp->type, &tmp->x, &tmp->y, &tmp->width, &tmp->height, &tmp->color)) == 6)
	{
		if (!check_shape(tmp))
			return (0);
		draw_shape(drawing, &tmp, zone);
	}
	if (scan_ret != -1)
		return (0);
	return (1);
}

void
	draw_shape(char drawing[900], t_shape *shape, t_zone *zone)
{
	int	i;
	int	j;
	int	start_x;
	int	start_y;
	int	lim_x;
	int	lim_y;

	start_x = ft_floor(shape->x); //ft_ceil(shape->x);
	start_y = ft_floor(shape->y); //ft_ceil(shape->y);
	lim_x = ft_floor(shape->x + shape->width);
	lim_y = ft_floor(shape->y + shape->height);
	i = start_y;
	while (i <= lim_y)
	{
		j = start_x;
		while (j <= lim_x)
		{
			if (i >= 0 && i < zone->height
				&& j >= 0 && j < zone->width
				&& (shape->type == 'R'
					|| shape->y - shape->height < 1.00000000
					|| (shape->y + shape->height) - (double)i < 1.00000000
					|| shape->x - shape->width < 1.00000000
					|| (shape->x + shape->width) - (double)j < 1.00000000)))
				drawing[(i * zone->width) + j] = shape->color;
			j++;
		}
		i++;
	}
}

void
	draw_drawing(char drawing[900], t_zone *zone)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int
	main(int argc, char **argv)
{
	t_zone	zone;
	char	drawing[900];
	FILE	*file;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	shapes = NULL;
	if (argc != 2)
		return (str_error("Error: argument\n", 1));
	if (!(file = fopen(argv[1], "r")))
		return (str_error("Error: Operation file corrupted\n", 1));
	if (!get_zone(file, &zone))
		return (clear_all(file) && str_error("Error: Operation file corrupted\n", 1));
	paint_background(drawing, &zone);
	if (!draw_shapes(file, drawing, &zone))
		return (clear_all(file) && str_error("Error: Operation file corrupted\n", 1));
	draw_drawing(drawing, &zone);
	clear_all(file);
	return (0);
}
