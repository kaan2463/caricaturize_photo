//============================================================================
// Name        : caricaturize.cpp
// Author      : Khan
// Version     :
// Copyright   : There is no copyright case, you can do whatever you want :)
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

struct rgb {
	int r;
	int g;
	int b;
};

void readPPM(const char *fileName, char *pSix, int *width, int *height,
		int *maximum, struct rgb **&data) {
	FILE *fr = fopen(fileName, "rb");
	fscanf(fr, "%s", pSix);
	if (strncmp(pSix, "P6", 10) != 0) {
		printf("They are not the same\n");
	} else {
		printf("They are the same\n");
	}
	fscanf(fr, "%d\n %d\n", width, height);
	fscanf(fr, "%d\n", maximum);
	printf("PSix: %s\n", pSix);
	printf("Width: %d\n", *width);
	printf("Height: %d\n", *height);
	printf("maximum: %d\n", *maximum);

	data = (struct rgb**) malloc(sizeof(struct rgb*) * (*height));
	for (int i = 0; i < (*height); i++) {
		data[i] = (struct rgb*) malloc(sizeof(struct rgb) * (*width));
	}

	int c;
	int i = 0, index = 0;
	while (fscanf(fr, "%c", &c) != EOF) {
		index = i / 3;
		if (i % 3 == 0) {
			data[index / (*width)][index % (*width)].r = c;
		} else if (i % 3 == 1) {
			data[index / (*width)][index % (*width)].g = c;
		} else if (i % 3 == 2) {
			data[index / (*width)][index % (*width)].b = c;
		}

		i++;
	}

	cout << endl << "sfsdf " << i << endl;

	fclose(fr);
}
void fill_histogram(struct rgb **data, int height, int width, rgb *histogram) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			histogram[data[i][j].r].r++;
			histogram[data[i][j].g].g++;
			histogram[data[i][j].b].b++;
		}
	}
}

void regulate_histogram(rgb *source_hist, rgb *dest_hist, int maximum,
		int step_length) {
	int densest_value_r = -1;
	int densest_value_g = -1;
	int densest_value_b = -1;
	for (int step = 0; step < maximum; step += step_length) {
		for (int i = step; i < step + step_length && i < maximum; i++) {
			if (densest_value_r == -1) {
				densest_value_r = i;
			}
			if (densest_value_g == -1) {
				densest_value_g = i;
			}
			if (densest_value_b == -1) {
				densest_value_b = i;
			}
			if (source_hist[densest_value_r].r < source_hist[i].r) {
				densest_value_r = i;
			}
			if (source_hist[densest_value_g].g < source_hist[i].g) {
				densest_value_g = i;
			}
			if (source_hist[densest_value_b].b < source_hist[i].b) {
				densest_value_b = i;
			}
		}

		for (int i = step; i < step + step_length && i < maximum; i++) {
			dest_hist[i].r = densest_value_r;
			dest_hist[i].g = densest_value_g;
			dest_hist[i].b = densest_value_b;
		}

		densest_value_r = -1;
		densest_value_g = -1;
		densest_value_b = -1;
	}

}

void apply_smooth_filter(struct rgb **&data, int height, int width) {
	int sum_r = 0;
	int sum_g = 0;
	int sum_b = 0;
	int count = 0;
	rgb temp[height][width] = { };
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < height && l + j > 0
							&& l + j < width) {
						sum_r += data[i + k][j + l].r;
						sum_g += data[i + k][j + l].g;
						sum_b += data[i + k][j + l].b;
						count++;
					}

				}
			}

			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < height && l + j > 0
							&& l + j < width) {
						temp[i + k][j + l].r = sum_r / count;
						temp[i + k][j + l].g = sum_g / count;
						temp[i + k][j + l].b = sum_b / count;
					}

				}
			}
			sum_r = 0;
			sum_g = 0;
			sum_b = 0;
			count = 0;
		}

	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data[i][j].r = temp[i][j].r;
			data[i][j].g = temp[i][j].g;
			data[i][j].b = temp[i][j].b;
		}
	}
}

void apply_gaussian_filter(struct rgb **&data, int height, int width) {
	int sum_r = 0;
	int sum_g = 0;
	int sum_b = 0;
	rgb temp[height][width] = { };
	int gaussian_filter[3][3] = { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < height && l + j > 0
							&& l + j < width) {
						sum_r += data[i + k][j + l].r
								* gaussian_filter[k + 1][l + 1];
						sum_g += data[i + k][j + l].g
								* gaussian_filter[k + 1][l + 1];
						sum_b += data[i + k][j + l].b
								* gaussian_filter[k + 1][l + 1];
					}

				}
			}

			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < height && l + j > 0
							&& l + j < width) {
						temp[i + k][j + l].r = sum_r / 16;
						temp[i + k][j + l].g = sum_g / 16;
						temp[i + k][j + l].b = sum_b / 16;
					}

				}
			}
			sum_r = 0;
			sum_g = 0;
			sum_b = 0;
		}

	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data[i][j].r = temp[i][j].r;
			data[i][j].g = temp[i][j].g;
			data[i][j].b = temp[i][j].b;
		}
	}
}

void apply_sobel_filter(struct rgb **&data, int height, int width) {
	int sum_r_x = 0;
	int sum_g_x = 0;
	int sum_b_x = 0;

	int sum_r_y = 0;
	int sum_g_y = 0;
	int sum_b_y = 0;
	rgb temp[height][width] = { };
	int sobel_filter_x[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
	int sobel_filter_y[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < height && l + j > 0
							&& l + j < width) {
						sum_r_x += data[i + k][j + l].r
								* sobel_filter_x[k + 1][l + 1];
						sum_g_x += data[i + k][j + l].g
								* sobel_filter_x[k + 1][l + 1];
						sum_b_x += data[i + k][j + l].b
								* sobel_filter_x[k + 1][l + 1];

						sum_r_y += data[i + k][j + l].r
								* sobel_filter_y[k + 1][l + 1];
						sum_g_y += data[i + k][j + l].g
								* sobel_filter_y[k + 1][l + 1];
						sum_b_y += data[i + k][j + l].b
								* sobel_filter_y[k + 1][l + 1];
					}

				}
			}

			for (int k = -1; k < 2; k++) {
				for (int l = -1; l < 2; l++) {
					if (i + k > 0 && i + k < height && l + j > 0
							&& l + j < width) {
						temp[i + k][j + l].r = abs(sum_r_x) + abs(sum_r_y);
						temp[i + k][j + l].g = abs(sum_g_x) + abs(sum_g_y);
						temp[i + k][j + l].b = abs(sum_b_x) + abs(sum_b_y);
					}

				}
			}
			sum_r_x = 0;
			sum_g_x = 0;
			sum_b_x = 0;

			sum_r_y = 0;
			sum_g_y = 0;
			sum_b_y = 0;
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (temp[i][j].r > 255) {
				data[i][j].r = 0;
			}
			if (temp[i][j].g > 255) {
				data[i][j].g = 0;
			}
			if (temp[i][j].b > 255) {
				data[i][j].b = 0;
			}
		}
	}

}

int main() {
	char *pSix = new char[30];
	int width, height, maximum;
	struct rgb **data;
	readPPM("/home/khan/Desktop/aa.ppm", pSix, &width, &height, &maximum, data);
	maximum++;

	struct rgb color_histogram[maximum] = { };
	struct rgb regulated_color_histogram[maximum] = { };

	FILE *fr = fopen("hello.ppm", "w");
	fprintf(fr, "%s", "P6\n");
	fprintf(fr, "%d\n %d\n", width, height);
	fprintf(fr, "%d\n", maximum - 1);
	fill_histogram(data, height, width, color_histogram);
	apply_smooth_filter(data, height, width);
	apply_sobel_filter(data, height, width);

	apply_gaussian_filter(data, height, width);
	regulate_histogram(color_histogram, regulated_color_histogram, maximum, 64);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			data[i][j].r = regulated_color_histogram[data[i][j].r].r;
			data[i][j].g = regulated_color_histogram[data[i][j].g].g;
			data[i][j].b = regulated_color_histogram[data[i][j].b].b;
		}
	}
	apply_sobel_filter(data, height, width);

	for (int i = 0; i < maximum; i++) {
		cout << regulated_color_histogram[i].r << " ";
	}
	cout << endl;
	for (int i = 0; i < maximum; i++) {
		cout << regulated_color_histogram[i].g << " ";
	}
	cout << endl;
	for (int i = 0; i < maximum; i++) {
		cout << regulated_color_histogram[i].b << " ";
	}
	cout << endl;

	int count = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			fprintf(fr, "%c", data[i][j].r);
			fprintf(fr, "%c", data[i][j].g);
			fprintf(fr, "%c", data[i][j].b);
			count++;
		}
	}
	cout << "cont " << count << " " << width << " " << height << endl;
	fclose(fr);
	return 0;
}
