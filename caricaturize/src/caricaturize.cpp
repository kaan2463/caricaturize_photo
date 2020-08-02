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

void applyFilter(struct rgb **&data, int height, int width,
		void (*filter_func)(rgb*)) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			filter_func(&data[i][j]);
		}
	}
}

void simple_filter(rgb *color) {
	color->r = color->r > 50 ? 255 : 0;
	color->g = color->g > 50 ? 255 : 0;
	color->b = color->b > 50 ? 255 : 0;
}

int main() {
	char *pSix = new char[30];
	int width, height, maximum;
	struct rgb **data;
	readPPM("/home/khan/Desktop/khan.ppm", pSix, &width, &height, &maximum,
			data);

	struct rgb color_histogram[maximum] = { };

	FILE *fr = fopen("hello.ppm", "w");
	fprintf(fr, "%s", "P6\n");
	fprintf(fr, "%d\n %d\n", width, height);
	fprintf(fr, "%d\n", maximum);

	fill_histogram(data, height, width, color_histogram);

	for (int i = 0; i < maximum; i++) {
		cout << color_histogram[i].r << " ";
	}
	cout << endl;
	for (int i = 0; i < maximum; i++) {
		cout << color_histogram[i].g << " ";
	}
	cout << endl;
	for (int i = 0; i < maximum; i++) {
		cout << color_histogram[i].g << " ";
	}
	cout << endl;

	applyFilter(data, height, width, simple_filter);

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
