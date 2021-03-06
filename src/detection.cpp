/*
 * main destiné à l'application de détection
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string>

#include "hog.hpp"
#include "image.hpp"

using namespace std;

void write_detection(HOG* hog, string filename);
void detect_image_window(string imagePath, string svmPath, string windowPath);

int main(int argc, char* argv[])
{

	if (argc==2)
	{
		string path(argv[1]);
		string svm(path+"/thog2.svm");
		string testImage(path+"/test/test5.raw");
		string windowOutput(path+"/test/test5.rect");

		detect_image_window(testImage,svm,windowOutput);
	}
	else
	{
		printf("Argument must be the path to the database directory");
	}
}

void detect_image_window(string imagePath, string svmPath, string windowPath)
{
	bool detected=false;

	Image img(imagePath);
	SVM svm(svmPath);

	HOG hog(&img);
	hog.svm = &svm;

	printf("Calculating gradients...\n");
	hog.calculate_gradient();
	printf("Calculating cells (building histograms)...\n");
	hog.calculate_cells();
	printf("Calculating blocks (normalization)...\n");
	hog.calculate_blocks();
	printf("Calculating windows (svm product)...\n");
	hog.calculate_windows();
	hog.printDetection();

	//write_detection(&hog,windowPath);
}

void write_detection(HOG* hog, string filename)
{
	FILE * fd;

	fd=fopen(filename.c_str(), "wb" );

	if(fd==NULL){
		perror("fopen()");
		exit(-1);
	}

	unsigned int winWrite=0;
	for (unsigned int i=0;i<hog->win_size_y;i++)
	{
		for (unsigned int j=0;j<hog->win_size_x;j++)
		{
			if (hog->detection[(i*hog->win_size_x)+j])
			{
				fwrite(&i,sizeof(int),1,fd);
				fwrite(&j,sizeof(int),1,fd);
				winWrite++;
			}
		}
	}

	printf("%d window(s) written to %s\n",winWrite,filename.c_str());

	//for (unsigned int i=0;i<10;i++) {
	//	printf("%f\n",win->values[i]);
	//}

	fclose(fd);

	return;
}
