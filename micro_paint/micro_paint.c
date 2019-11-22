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
	float	x;
	float	y;
	float	width;
	float	height;
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
	ft_floor(float val)
{
	return ((int)val);
}

int
	ft_ceil(float val)
{
	int	abs;

	abs = (int)val;
	if (val - abs > 0)
		return ((int)((float)1. + (float)val));
	return (abs);
}

int
	str_error(char const *str, int ret)
{
	write(1, str, ft_strlen(str));
	return (ret);
}

int
	clear_all(FILE *file, t_shape **shapes, char *drawing)
{
	t_shape	*tmp;

	fclose(file);
	while (shapes && *shapes)
	{
		tmp = (*shapes)->next;
		free(*shapes);
		*shapes = tmp;
	}
	if (drawing)
		free(drawing);
	return (1);
}

t_shape
	*new_shape(void)
{
	t_shape	*new;

	if (!(new = (t_shape*)malloc(sizeof(*new))))
		return (NULL);
	new->type = 0;
	new->x = 0.;
	new->y = 0.;
	new->width = 0.;
	new->height = 0.;
	new->color = 0;
	new->next = 0;
	return (new);
}

int
	check_zone(t_zone *zone)
{
	return (zone->width > 0 && zone->width <= 300
			&& zone->height > 0 && zone->height <= 300
			/*&& (unsigned char)zone->background >= 0*/);
}

int
	check_shape(t_shape *shape)
{
	return (shape->width > 0 && shape->height > 0 /*&& (unsigned char)shape->color >= 0*/
			&& (shape->type == 'r' || shape->type == 'R')
			&& shape->color != ' ');
}

int
	parse_file(FILE *file, t_zone *zone, t_shape **shapes)
{
	t_shape	*tmp;
	int		total;
	t_shape	*first;
	int		scan_ret;

	first = NULL;
	total = 0;
	if (!(tmp = new_shape()))
		return (0);
	if ((scan_ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->background)) != 3)
		return (0);
	if (!check_zone(zone))
		return (0);
	if (scan_ret == -1)
		return (0);
	while ((scan_ret = fscanf(file, "%c %f %f %f %f %c\n", &tmp->type, &tmp->x, &tmp->y, &tmp->width, &tmp->height, &tmp->color)) == 6)
	{
		if (!*shapes)
			first = tmp;
		else
			(*shapes)->next = tmp;
		*shapes = tmp;
		if (!check_shape(tmp))
			return (0);
		if (!(tmp = new_shape()))
			return (0);
		total++;
	}
	if (total > 1)
		free(tmp);
	if (scan_ret != -1)
		return (0);
	*shapes = first;
	return (1);
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
	draw_shape(char *drawing, t_shape *shape, t_zone *zone)
{
	int	i;
	int	j;
	int	start_x;
	int	start_y;
	int	lim_x;
	int	lim_y;

	start_x = ft_ceil(shape->x);
	start_y = ft_ceil(shape->y);
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
				&& (shape->type == 'R' || (i == start_y || i == lim_y
					|| j == start_x || j == lim_x)))
				drawing[(i * zone->width) + j] = shape->color;
			j++;
		}
		i++;
	}
}

void
	draw_drawing(char *drawing, t_zone *zone)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		printf("%.*s\n", zone->width, drawing + (i * zone->width));
		/*write(1, drawing + (i * zone->width), zone->width);
		write(1, "\n", 1);*/
		i++;
	}
}

int
	main(int argc, char **argv)
{
	t_zone	zone;
	t_shape	*shapes;
	char	*drawing;
	t_shape	*tmp;
	FILE	*file;

	zone.width = 0;
	zone.height = 0;
	zone.background = 0;
	shapes = NULL;
	drawing = NULL;
	if (argc != 2)
		return (str_error("Error: argument\n", 1));
	if (!(file = fopen(argv[1], "r")))
		return (str_error("Error: Operation file corrupted\n", 1));
	if (!parse_file(file, &zone, &shapes))
		return (clear_all(file, &shapes, drawing) && str_error("Error: Operation file corrupted\n", 1));
	if (!(drawing = new_drawing(&zone)))
		return (clear_all(file, &shapes, drawing) && str_error("Error: malloc failed :)\n", 1));
	while (shapes)
	{
		draw_shape(drawing, shapes, &zone);
		tmp = shapes->next;
		free(shapes);
		shapes = tmp;
	}
	draw_drawing(drawing, &zone);
	clear_all(file, NULL, drawing);
	return (0);
}
