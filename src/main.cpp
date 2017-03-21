/*
 * Fichier du laboratoire 2 du lab INF8505
 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string>
#include <dirent.h>

#include "hog.hpp"
#include "image.hpp"

using namespace std;

char* read_image(const char* file_name, int* row_size, int* col_size);
void write_image(char* image_out, string filename, int row_size, int col_size);
void write_image(float* image_out, string filename, int row_size, int col_size);
void write_cells(Histogram* hists, string filename, int y_size, int x_size);
void write_blocks(NormalizedHistogram* hists, string filename, int y_size, int x_size);
void write_windows(HOG* hog, string filename);
void write_detection(HOG* hog, string filename);
void compute_image_window(string rawImageFilename, string outputFilename);
void compute_all_image_window(string inDir, string outDir);
void detect_image_window(string imagePath, string svmPath, string windowPath);
void histogramTest(string rawImageFilename, string outputFilename);

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

void histogramTest(string rawImageFilename, string outputPath)
{
	char* image_in;
	int row_size, col_size;

	//Reading input image
	image_in=read_image(rawImageFilename.c_str(), &row_size, &col_size);

	HOG hog(image_in,row_size,col_size);

	printf("Image %s has %d window(s)\n",rawImageFilename.c_str(),hog.win_size);

	hog.calculate_gradient();
	hog.calculate_cells();
	hog.calculate_blocks();
	hog.calculate_windows();

	write_blocks(hog.blocks, outputPath+"/histogram", hog.block_size_y, hog.block_size_x);
	write_image(image_in, outputPath+"/image", row_size, col_size);
	write_image(hog.norm, outputPath+"/norm", row_size, col_size);

	free(image_in);
}


int main(void)
{
	string svm="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/thog2.svm";
	//string testImage="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/test/test2.raw";
	string testImage="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/raw/neg/00000002a.raw";
	//string testImage="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/raw/pos/per00001.raw";
	string testOutput="/home/kakaroth/Ecole/INF8505/projet/workspace/hog";
	string windowOutput="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/test/test2.rect";

	//histogramTest(testImage,testOutput);
	detect_image_window(testImage,svm,windowOutput);

	//positive sample
	string input_dir="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/raw/pos";
	string output_dir="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/window/pos";

	//compute_all_image_window(input_dir,output_dir);

	//negative sample
	input_dir="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/raw/neg";
	output_dir="/home/kakaroth/Ecole/INF8505/projet/THOG-life/database/window/neg";

	//compute_all_image_window(input_dir,output_dir);
}

void compute_all_image_window(string inDir, string outDir)
{
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(inDir.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type==DT_REG)
			{
				string inName = ent->d_name;
				string outName = inName.substr(0,inName.find("."))+".hogwin";

				string inPath = inDir+"/"+inName;
				string outPath = outDir+"/"+outName;

				compute_image_window(inPath,outPath);

				//printf("%s -> %s\n", fullname.c_str(),(name+".hogwin").c_str());
			}
		}
		closedir(dir);
	}
	else
	{
		/* could not open directory */
	}
}


void compute_image_window(string rawImageFilename, string outputFilename)
{
	char* image_in;
	int row_size, col_size;

	//Reading input image
	image_in=read_image(rawImageFilename.c_str(), &row_size, &col_size);

	HOG hog(image_in,row_size,col_size);

	printf("Image %s has %d window(s)\n",rawImageFilename.c_str(),hog.win_size);

	hog.calculate_gradient();
	hog.calculate_cells();
	hog.calculate_blocks();
	hog.calculate_windows();

	write_windows(&hog, outputFilename);

	free(image_in);
}

/*----------------------------------------------------------------------------*/
// Reading input image file (raw format)

char* read_image(const char* file_name, int* row_size, int* col_size)
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

void write_blocks(NormalizedHistogram* hists, string filename, int y_size, int x_size)
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
		fwrite(hists[i].hist,sizeof(float),36,fd);
	}

	fclose(fd);

	return;
}

void write_windows(HOG* hog, string filename)
{
	FILE * fd;

	fd=fopen(filename.c_str(), "wb" );

	if(fd==NULL){
		perror("fopen()");
		exit(-1);
	}

	//writing the size (number of windows in the image)
	fwrite(&hog->win_size,sizeof(int),1,fd);

	//writing each window vector back to back
	for (unsigned int i=0;i<hog->win_size;i++)
	{
		fwrite(hog->windows[i].values,sizeof(float),3780,fd);
	}

	printf("%d window(s) written to %s\n",hog->win_size,filename.c_str());

	//for (unsigned int i=0;i<10;i++) {
	//	printf("%f\n",win->values[i]);
	//}

	fclose(fd);

	return;
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
