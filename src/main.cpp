/*
 * Fichier du laboratoire 2 du lab INF8505
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string>
#include "hog.hpp"
#include "svm.hpp"

using namespace std;

char* Sobel(char* image_in, int row_size, int col_size);
char* read_image(char* file_name, int* row_size, int* col_size);
void write_image(char* image_out, string filename, int row_size, int col_size);
void write_image(float* image_out, string filename, int row_size, int col_size);
void write_cells(Histogram* hists, string filename, int y_size, int x_size);

int main(void)
{
    char* image_in;
    char* image_out;
    int i;
    int row_size, col_size;

    //dummy svm pour le moment
    SVM dummySVM;

    //Reading input image
	printf("\n\nReading input image file ...\n\n");
    image_in=read_image("image_in.rimg", &row_size, &col_size);
	printf("It is a %d x %d pixel image \n\n", row_size, col_size);
    

	//Coeur du programme, faire la reconnaissance de pi�ton sur l'image image_in
	//xt_profile_enable();
    //image_out= Sobel(image_in, row_size, col_size);

	HOG myHog(image_in,row_size,col_size);
	myHog.calculate_gradient();
	myHog.calculate_cells();
	myHog.calculate_blocks();
	myHog.calculate_windows(&dummySVM);

	//xt_profile_disable();

    printf("Writing the processed image in output file ...\n\n");
    //getch();
    
    //write_image(myHog.image, "ori.out", row_size, col_size);
    //write_image(myHog.xGrad, "xgrad.out", row_size, col_size);
    //write_image(myHog.yGrad, "ygrad.out", row_size, col_size);
    //write_image(myHog.norm, "norm.out", myHog.row_size, myHog.col_size);
    //write_image(myHog.angle, "angle.out", myHog.row_size, myHog.col_size);

    //write_cells(myHog.cells, "hist.out", myHog.cell_size_y, myHog.cell_size_x);

    // free allocated memories
    free(image_in);
    free(image_out);

    printf("Done\n\n");

    return 0;
}


/*----------------------------------------------------------------------------*/
// Reading input image file (raw format)

char* read_image(char* file_name, int* row_size, int* col_size)
{

    FILE* fd;
    int R_size, C_size, img_size;
    char* buf;

    fd=fopen( file_name, "rb" );

    if(fd==NULL){
        perror("fopen()");
        exit(-1);
    }

    //Reading the image size
    fread(&R_size,sizeof(int),1,fd);
    fread(&C_size,sizeof(int),1,fd);

    img_size=R_size*C_size;

    buf = (char*)malloc(img_size*sizeof(char));

    fread(buf,1,img_size,fd);

    *row_size=R_size;
    *col_size=C_size;

    fclose(fd);

    return buf;
}



/*----------------------------------------------------------------------------*/
// Writes the output image in a file

void write_image(char* image_out, string filename, int row_size, int col_size)
{

    FILE * fd;

    fd=fopen(filename.c_str(), "wb" );

    if(fd==NULL){
        perror("fopen()");
        exit(-1);

    }

    //writing the image size
    fwrite(&row_size,sizeof(int),1,fd);
    fwrite(&col_size,sizeof(int),1,fd);

    //writing pixels
    fwrite(image_out,sizeof(char),col_size*row_size,fd);

    fclose(fd);

    return;
}

void write_image(float* image_out, string filename, int row_size, int col_size)
{

	char* buf = (char*)malloc(row_size*col_size*sizeof(char));
	int i;

	for (i=0;i<row_size*col_size;i++)
	{
		buf[i] = (char)image_out[i];
	}

	write_image(buf,filename,row_size,col_size);

	free(buf);

    return;
}

void write_cells(Histogram* hists, string filename, int y_size, int x_size)
{
	FILE * fd;

	fd=fopen(filename.c_str(), "wb" );

	if(fd==NULL){
		perror("fopen()");
		exit(-1);
	}

	//writing the size
	fwrite(&y_size,sizeof(int),1,fd);
	fwrite(&x_size,sizeof(int),1,fd);

	//writing histograms
	for (int i=0;i<y_size*x_size;i++)
	{
		fwrite(hists[i].hist,sizeof(float),9,fd);
	}

	fclose(fd);

	return;
}









/*
 * Fonction de filtrage de type Sobel du labo 2
 * Pas utilis� mais pr�sent � titre d'exemple
 */
char* Sobel(char* image_in, int row_size, int col_size)
{
	char* image_out;
	int x, y, i, j;  /* Loop variable */
    int pixel_value;
    int min, max;
	int weight[3][3] = {{ -1,  0,  1 },
			            { -2,  0,  2 },
	                    { -1,  0,  1 }};


	/* Definition of Sobel filter in horizontal direction */
	/* Maximum values calculation after filtering*/
	min = DBL_MAX;
	max = -DBL_MAX;

	for (y = 1; y < (int)row_size - 1; y++) {
		for (x = 1; x < (int)col_size - 1; x++) {
			pixel_value = 0.0;
			for (j = -1; j <= 1; j++) {
				for (i = -1; i <= 1; i++) {
					pixel_value += weight[j + 1][i + 1] * image_in[(y+j)*col_size+(x+i)];
					//pixel_value = weight[j + 1][i + 1] * image_in[y + j][x + i];
				}
			}
			if (pixel_value < min) min = pixel_value;
			if (pixel_value > max) max = pixel_value;
			//printf("\n R= %d, C= %d  min=%u, max=%u", y, x, min, max);
		}
	}
	//printf("\n *****min=%d    max= %d", min, max);
	if ((int)(max - min) == 0) {
		printf("Nothing exists!!!\n\n");
		exit(1);
	}

	/* Initialization of image2[y][x] */
	image_out = (char*)malloc(row_size*col_size*sizeof(char));

	for (y = 0; y < (int) row_size; y++) {
		for (x = 0; x < (int) col_size; x++) {
			image_out[y*col_size+x] = 0;
		}
	}
	/* Generation of image2 after linear transformtion */
	for (y = 1; y < (int) row_size - 1; y++) {
		for (x = 1; x < (int) col_size - 1; x++) {
			pixel_value = 0.0;
			for (j = -1; j <= 1; j++) {
				for (i = -1; i <= 1; i++) {
                pixel_value += weight[j + 1][i + 1] * image_in[(y+j)*col_size+(x+i)];
				}
			}
			pixel_value = 255 * (pixel_value - min) / (max - min);
			image_out[y*col_size+x] = (unsigned char)pixel_value;
		}
	}

	return (image_out);
}
