/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_example.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncolomer <ncolomer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/22 13:09:44 by ncolomer          #+#    #+#             */
/*   Updated: 2019/11/23 16:29:24 by ncolomer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <random>
#include <chrono>

#define FULL_ASCII 0
#define ASCII_RANGE (FULL_ASCII) ? 1 : 32, (FULL_ASCII) ? 255 : 126
#define MODE "rR"

int rand_int_range(int min, int max, std::mt19937 &rng)
{
	return (std::uniform_int_distribution<int>(min, max)(rng));
}

float rand_float_range(float min, float max, std::mt19937 &rng)
{
	return (std::uniform_real_distribution<float>(min, max)(rng));
}

int main(void)
{
	FILE *file;
	int width, height, background;
	int i, nbr_lines;
	int type, color;
	float x, y, swidth, sheight;
	int size;
	char spaces[7][5];

	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
	if (!(file = fopen("example_", "w")))
		return (printf("file: fopen error.\n"));

	// Parameters
	width = rand_int_range(-1, 301, rng);
	height = rand_int_range(-1, 301, rng);
	background = rand_int_range(ASCII_RANGE, rng);
	if (fprintf(file, "%d %d %c\n", width, height, background) < 0)
		return (fclose(file) && printf("file: fprintf error.\n"));

	// Generate each lines
	nbr_lines = rand_int_range(-2, 42, rng);
	i = 0;
	while (i++ < nbr_lines)
	{
		// 10% empty lines, normal line otherwise
		if (rand_int_range(0, 100, rng) < 90)
		{
			// Shape type, 49.5% for each, 1% errors
			type = rand_int_range(0, 10000, rng);
			if (type >= 9900)
				type = 'a';
			else if (type >= 4950)
				type = 'r';
			else
				type = 'R';
			color = rand_int_range(ASCII_RANGE, rng);

			// Size, 75% above 0, 25% negative
			if ((size = rand_int_range(0, 100, rng)) >= 25)
			{
				x = rand_float_range(.001, (float)size, rng);
				y = rand_float_range(.001, (float)size, rng);
				swidth = rand_float_range(.001, (float)size, rng);
				sheight = rand_float_range(.001, (float)size, rng);
			}
			else
			{
				x = rand_float_range(-100., 400., rng);
				y = rand_float_range(-100., 400., rng);
				swidth = rand_float_range(-.90, 400., rng);
				sheight = rand_float_range(-.90, 400., rng);
			}

			// Generate spaces between parameters
			for (size_t j = 0; j < 6; j++)
			{
				if (rand_int_range(0, 100, rng) >= 75)
				{
					int count = rand_int_range(1, 5, rng);
					int k = 0;
					for (; k < count; k++)
					{
						spaces[j][k] = ' ';
					}
					spaces[j][k] = 0;
				}
				else if (j == 0 || j == 5)
				{
					spaces[j][0] = 0;
				}
				else
				{
					spaces[j][0] = ' ';
					spaces[j][1] = 0;
				}
			}

			// Print to file, 65% float values, 35% rounded to int
			if (rand_int_range(0, 100, rng) >= 35)
				size = fprintf(file, "%s%c%s%f%s%f%s%f%s%f%s%c%s", spaces[0], type, spaces[1], x, spaces[2], y, spaces[3], swidth, spaces[4], sheight, spaces[5], color, spaces[6]);
			else
				size = fprintf(file, "%s%c%s%d%s%d%s%d%s%d%s%c%s", spaces[0], type, spaces[1], (int)x, spaces[2], (int)y, spaces[3], (int)swidth, spaces[4], (int)sheight, spaces[5], color, spaces[6]);
			if (i < nbr_lines && size)
				size = fprintf(file, "\n");
		}
		else
		{
			size = fprintf(file, "\n");
			nbr_lines++;
		}

		if (size < 0)
			return (fclose(file) && printf("file: fprintf error.\n"));
	}
	fclose(file);
	return (0);
}
