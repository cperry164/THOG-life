#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "hog.hpp"


#define PI 3.14159265359
#define EPSILLON std::numeric_limits<float>::epsilon()

/**
 * TODO: ajouter des vérifications comme quoi tous les size sont positives
 */
HOG::HOG(char* image, int row_size, int col_size, SVM* svm)
{
	//Initialisation image
	this->image = image;
	this->row_size = row_size;
	this->col_size = col_size;
	this->img_size = row_size*col_size;

	//svm
	this->svm = svm;

	//Les cellules font 8x8 pixels
	this->cell_size_x = col_size/8;
	this->cell_size_y = row_size/8;
	this->cell_size = cell_size_x*cell_size_y;

	//Les blocs font 2X2 cellules. Il y a donc 1 bloc de moins que de cellule dans chaque directions
	this->block_size_x = (cell_size_x-1);
	this->block_size_y = (cell_size_y-1);
	this->block_size = block_size_x*block_size_y;

	//Une fenêtre fait 8 cellules par 16 cellules. Une image de 8X16 aura donc une seule fenêtre
	this->win_size_x = cell_size_x-7;
	this->win_size_y = cell_size_y-15;
	this->win_size = win_size_x*win_size_y;

	//On réserve beaucoup de mémoire
	xGrad = (char*)malloc(img_size*sizeof(char));
	yGrad = (char*)malloc(img_size*sizeof(char));
	norm = (float*)malloc(img_size*sizeof(float));
	angle = (float*)malloc(img_size*sizeof(float));
	detection = (bool*)malloc(win_size*sizeof(bool));

	cells = new Histogram[cell_size];
	blocks = new NormalizedHistogram[block_size];
	windows = new HogWindow[win_size];
}

HOG::HOG(Image* img, SVM* svm) : HOG(img->buffer,img->size_y,img->size_x,svm)
{
}

HOG::~HOG()
{
	free(xGrad);
	free(yGrad);
	free(norm);
	free(angle);
	free(detection);

	if (cells!=NULL)
	{
		delete[] cells;
	}
	if (blocks!=NULL)
	{
		delete[] blocks;
	}
	if (windows!=NULL)
	{
		delete[] windows;
	}
}

void HOG::calculate_gradient()
{
	unsigned int i, j,index;

	for (i=0;i<row_size;i++)
	{
		for (j=0;j<col_size;j++)
		{
			index = i*col_size+j;

			//gradient X
			if (j==0)
			{
				xGrad[index] = image[index+1]-image[index];
			}
			else if (j==(col_size-1))
			{
				xGrad[index] = image[index]-image[index-1];
			}
			else
			{
				xGrad[index] = image[index+1]-image[index-1];
			}

			//gradient y
			if (i==0)
			{
				yGrad[index] = image[index+col_size]-image[index];
			}
			else if (i==(row_size-1))
			{
				yGrad[index] = image[index]-image[index-col_size];
			}
			else
			{
				yGrad[index] = image[index+col_size]-image[index-col_size];
			}

			float xGradPow = ((float)xGrad[index])*xGrad[index];
			float yGradPow = ((float)yGrad[index])*yGrad[index];
			float gradDiv = ((float)yGrad[index])/((float)xGrad[index]+EPSILLON);

			norm[index] = sqrt(xGradPow+yGradPow);
			angle[index] = (atan(gradDiv)*180/PI)+90;
		}
	}
}

void HOG::calculate_cells()
{
	unsigned int i,j;
	int cellIndex,pixelIndex;

	cellIndex=0;
	pixelIndex=0;

	for (i=0;i<cell_size_y;i++)
	{
		for (j=0;j<cell_size_x;j++)
		{
			//cellIndex = (i*cell_size_x)+j;
			//pixelIndex = (i*col_size)+(j*8);

			cells[cellIndex].calculate_hist(&norm[pixelIndex],
								 	 	 	&angle[pixelIndex],
											col_size);
			cellIndex++;
			//go to the next first pixel of the next cell in the same cell row
			//which is 8 pixels to the right
			pixelIndex+=8;
		}
		//go to the next pixel of the nest cell in the same cell column
		//we already incremented pixelIndex by one col_size
		//we need to go 7 row further to reach the first pixel of the next row of cells
		pixelIndex+=(7*col_size);
	}
}

void HOG::calculate_blocks()
{
	unsigned int i,j;
	unsigned int cellIndex,blockIndex;
	std::list<const Histogram*> hlist;

	for (i=0;i<block_size_y;i++)
	{
		for (j=0;j<block_size_x;j++)
		{
			blockIndex = (i*block_size_x)+j;
			cellIndex = (i*cell_size_x)+j;

			hlist.clear();
			hlist.push_back(&cells[cellIndex]);
			hlist.push_back(&cells[cellIndex+1]);
			hlist.push_back(&cells[cellIndex+cell_size_x]);
			hlist.push_back(&cells[cellIndex+cell_size_x+1]);

			blocks[blockIndex].calculate_normedhist(hlist);
		}
	}
}

void HOG::calculate_windows()
{
	unsigned int i,j,m,n;
	unsigned int winIndex, blockIndex, winBlockIndex;
	std::list<const NormalizedHistogram*> *nhlist;

	//For each window in y
	for (i=0;i<win_size_y;i++)
	{
		//For each window in x
		for (j=0;j<win_size_x;j++)
		{
			//index of the first block in the window
			winIndex = i*win_size_x+j;
			winBlockIndex = i*block_size_x+j;

			//Each window contains a NormalizedHistogram list.
			//nhlist is a pointer to this list
			nhlist = &(windows[winIndex].nhists);

			//For each block of the window in y
			for (m=0;m<15;m++)
			{
				//For each block of the window in x
				for (n=0;n<7;n++)
				{
					//the index of the block in the global image block indexing system
					blockIndex=winBlockIndex+(m*block_size_x)+n;

					//add the block to the current window's block list
					nhlist->push_back(&blocks[blockIndex]);
				}
			}

			//Once a window is completed we can fill the values array
			//There might be some optimisation to do here since calculate_values() will
			//iterate over the nhlist we just built in the loop before
			windows[winIndex].calculate_values();

			//If we have an SVM, calculate the detection
			if (this->svm != NULL)
			{
				detection[winIndex] = windows[winIndex].detect(this->svm);
			}
		}
	}
}

void HOG::printDetection()
{
	unsigned int i;

	for (unsigned int i=0;i<win_size_y;i++)
	{
		for (unsigned int j=0;j<win_size_x;j++)
		{
			if (detection[(i*win_size_x)+j])
			{
				printf("Person detected in window x=%d,y=%d\n",j*8,i*8);
			}
		}
	}
	/*
	for (i=0;i<win_size;i++)
	{
		if (detection[i])
		{
			printf("Person detected in window %d\n",i);
		}
	}
	*/
}


/**
 *
 *
 * Histogram CLASS
 *
 *
 */
Histogram::Histogram()
{
	for (int i=0;i<9;i++)
	{
		hist[i]=0.0f;
	}
}

void Histogram::calculate_hist(const float* norm,const float* angle,int col_size)
{
	static const float binsAngle[9] = {0.0f, 20.0f, 40.0f, 60.0f, 80.0f,
									   100.0f, 120.0f, 140.0f, 160.0f};

	int i,j,index;
	char bin;
	float a,n;
	float excentricity, angleToNextBin;

	for (i=0;i<8;i++)
	{
		for (j=0;j<8;j++)
		{
			index=i*col_size+j;
			a = angle[index];
			n = norm[index];

			// simple decision tree to determine the bin
			if (a<=80) {
				if (a<=40) {
					if (a<=20) {
						bin=0;
					}
					else {
						bin=1;
					}
				}
				else {
					if (a<=60) {
						bin=2;
					}
					else {
						bin=3;
					}
				}
			}
			else {
				if (a<=120) {
					if (a<=100) {
						bin=4;
					}
					else {
						bin=5;
					}
				}
				else {
					if (a<=140) {
						bin=6;
					}
					else {
						if (a<=160) {
							bin=7;
						}
						else {
							bin=8;
						}
					}
				}
			}

			//excentricity is between 0 and 1
			excentricity = (a-binsAngle[bin])/20.0f;

			angleToNextBin = n*excentricity;

			//if the angle is above 160, the next bin is 0 degrees
			hist[(bin+1)%9]+=angleToNextBin;
			hist[bin]+=(n-angleToNextBin);
		}
	}
}

/**
 *
 *
 *
 * NormalizedHistogram Class
 *
 *
 */
NormalizedHistogram::NormalizedHistogram()
{
	l2hys = 0.0f;

	for (int i=0;i<36;i++)
	{
		hist[i]=0.0f;
	}
}

void NormalizedHistogram::calculate_normedhist(std::list<const Histogram*> hists)
{
	int i,j, index;

	for (std::list<const Histogram*>::iterator it=hists.begin(); it != hists.end(); it++)
	{
		for (j=0;j<9;j++)
		{
			l2hys += pow((*it)->hist[j],2);
		}
	}

	l2hys = sqrt(l2hys)+EPSILLON;

	index=0;
	for (std::list<const Histogram*>::iterator it=hists.begin(); it != hists.end(); it++)
	{
		for (j=0;j<9;j++)
		{
			this->hist[index] = ((*it)->hist[j])/l2hys;
			index++;
		}
	}
}



/*
 *
 * HogWindow CLASS
 *
 */
HogWindow::HogWindow()
{

}

void HogWindow::calculate_values()
{
	if (nhists.size()==105)
	{
		unsigned int index=0;

		for(std::list<const NormalizedHistogram*>::iterator it = nhists.begin();
				it != nhists.end(); it++)
		{
			for(unsigned int j=0;j<36;j++)
			{
				values[index]=(*it)->hist[j];
				index++;
			}
		}
	}
	else
	{
		printf("HogWindow trying to calculate values of a window with %d normalized histograms (105 required)",nhists.size());
	}
}

bool HogWindow::detect(const SVM* reference)
{
	unsigned int i;
	float dotProduct=0.0f;

	for (i=0;i<3780;i++)
	{
		dotProduct += this->values[i]*reference->values[i];
	}

	//printf("prod=%f",dotProduct);
	return (dotProduct<=-reference->bias);
}

/*
 *
 * SVM Class
 *
 */
SVM::SVM(std::string filepath)
{
	FILE* fd;

	fd=fopen(filepath.c_str(),"rb");

	if(fd==NULL){
		printf("Cannot open %s\n",filepath);
		return;
	}

	//Reading the file
	fread(&this->bias,sizeof(float),1,fd);
	fread(this->values,sizeof(float),3780,fd);

	fclose(fd);
}

