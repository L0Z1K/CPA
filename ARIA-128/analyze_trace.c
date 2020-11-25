#include <stdio.h>

int main(void) {
	unsigned char uchar[9];
	unsigned int n, m;
	FILE *file_pointer = fopen("problem/data/trace", "rb");

	// read header
	fread(uchar, 1, 8, file_pointer); uchar[8] = '\0';
	printf("%s\n", uchar);
	
	fread(&n, 4, 1, file_pointer);
	printf("%u traces\n", n);

	fread(&m, 4, 1, file_pointer);
	printf("%u samples\n", m);

	fread(&uchar, 1, 4, file_pointer); uchar[4] = '\0';
	printf("%s\n", uchar);

	FILE *data_pointer = NULL;
	// read body
    float data = 0;
	for (int i = 0; i < n; i++) {
        char name[100] = { 0 };
        sprintf(name, "problem/trace/%d.txt", i);
        data_pointer = fopen(name, "wb");
		for (int j = 0; j < m; j++) {
			fread(&data, 4, 1, file_pointer);
			fprintf(data_pointer, "%f\n", data);
		}
        fclose(data_pointer);
	}
	fclose(file_pointer);
}