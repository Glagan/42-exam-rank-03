#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct drawing drawing, *Pdrawing;

struct drawing { // Represent the width, height and matrice of pixels
    int width;
    int height;
    char * matrice;
};

typedef struct rectangle rectangle, *Prectangle;

struct rectangle { // A rectangle
    char type;
    float x;
    float y;
    float width;
    float height;
    char color;
};


int ft_strlen(char *str)

{
	int i;

	i = 0;
	while (str[i] != '\0') {
		i = i + 1;
	}
	return i;
}



int is_in_rectangle(float x,float y,rectangle *rectangle)

{
	int ret;

	if ((((x < rectangle->x) || (rectangle->x + rectangle->width < x)) || (y < rectangle->y)) ||
		(rectangle->y + rectangle->height < y)) {
		ret = 0;
	}
	else {
		if (((x - rectangle->x < 1.00000000) || ((rectangle->x + rectangle->width) - x < 1.00000000)) ||
			((y - rectangle->y < 1.00000000 || ((rectangle->y + rectangle->height) - y < 1.00000000)))) {
			ret = 2;
		}
		else {
			ret = 1;
		}
	}
	return ret;
}



int get_info(FILE *filep,drawing *drawing)

{
	int scan_ret;
	char *tmp;
	int i;
	char background;
	int ret;

	scan_ret = fscanf(filep,"%d %d %c\n",&drawing->width,&drawing->height,&background);
	if (scan_ret == 3) {
		if ((((drawing->width < 1) || (300 < drawing->width)) || (drawing->height < 1)) ||
			(300 < drawing->height)) {
			ret = 1;
		}
		else {
			tmp = (char *)malloc((long)drawing->width * (long)drawing->height);
			drawing->matrice = tmp;
			if (drawing->matrice == (char *)0x0) {
				ret = 1;
			}
			else {
				i = 0;
				while (i < drawing->width * drawing->height) {
				drawing->matrice[i] = background;
				i = i + 1;
				}
				ret = 0;
			}
		}
	}
	else {
		ret = 1;
	}
	return ret;
}



void execute_one(rectangle *rect, drawing *drawing,int x,int y)

{
	int is_in;

	is_in = is_in_rectangle((float)x,(float)y,rect);
	if ((is_in == 2) || ((is_in == 1 && (rect->type == 'R')))) {
		drawing->matrice[x + y * drawing->width] = rect->color;
	}
	return;
}



int apply_op(rectangle *rect,drawing *drawing)

{
	int j;
	int i;
	int ret;

	if (((rect->width <= 0.00000000) || (rect->height <= 0.00000000)) ||
		((rect->type != 'R' && (rect->type != 'r')))) {
		ret = 1;
	}
	else {
		i = 0;
		while (i < drawing->width) {
		j = 0;
		while (j < drawing->height) {
			execute_one(rect,drawing,i,j);
			j = j + 1;
		}
		i = i + 1;
		}
		ret = 0;
	}
	return ret;
}



int print_info(drawing *zone)

{
	int line;

	line = 0;
	while (line < zone->height) {
		printf("%.*s\n",zone->width,zone->matrice + line * zone->width);
		line = line + 1;
	}
	return line;
}



int execute(FILE *filep)

{
	int valid_zone;
	int fscanf_ret;
	rectangle rect;
	drawing screen;
	int ret;

	valid_zone = get_info(filep, &screen);
	if (valid_zone == 0) {
		fscanf_ret = fscanf(filep,"%c %f %f %f %f %c\n",&rect.type,&rect.x,&rect.y,&rect.width,&rect.height,&rect.color);
		while (fscanf_ret == 6) {
			valid_zone = apply_op(&rect,&screen);
			if (valid_zone != 0) {
				return 1;
			}
			fscanf_ret = fscanf(filep,"%c %f %f %f %f %c\n",&rect.type,&rect.x,&rect.y,&rect.width,&rect.height,&rect.color);
		}
		if (fscanf_ret == -1) {
			print_info(&screen);
			ret = 0;
		}
		else {
			ret = 1;
		}
	}
	else {
		ret = 1;
	}
	return ret;
}



int main(int argc,char **argv)

{
	int   i;
	FILE  *file;
	int   ret;

	ret = 0;
	if (argc == 2) {
		file = fopen(argv[1],"r");
		if ((file != (FILE *)0x0) && (i = execute(file), i == 0))
			return 0;
		i = ft_strlen("Error: Operation file corrupted\n");
		write(1,"Error: Operation file corrupted\n",(long)i);
	}
	else {
		i = ft_strlen("Error: argument\n");
		write(1,"Error: argument\n",(long)i);
		ret = 1;
	}
	return ret;
}
