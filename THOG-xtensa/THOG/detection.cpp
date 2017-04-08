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

int main(void)
{
	string svm="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/thog2.svm";
	string testImage="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/test/test5.raw";
	string windowOutput="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/test/test5.rect";

	detect_image_window(testImage,svm,windowOutput);
}

void detect_image_window(string imagePath, string svmPath, string windowPath)
{
	bool detected=false;

	Image img(imagePath);
	SVM svm(svmPath);

	HOG hog(&img);
	hog.svm = &svm;

	hog.calculate_gradient();
	hog.calculate_cells();
	hog.calculate_blocks();
	hog.calculate_windows();
	hog.printDetection();

	write_detection(&hog,windowPath);
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
