#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits>
#include "hog.hpp"
#include "cordic.hpp"

//#define CHECK4ERRORS

#define TIE
#ifdef TIE
	#include "C:\TEMP\THOG-life\THOG-xtensa\XtensaInfo\Models\tie_dev1.tdk\include\xtensa\tie\first_tie.h"
#endif

#define PI 3.14159265359
#define EPSILLON std::numeric_limits<float>::epsilon()
#define EPSILLON_INT 1

/**
 * TODO: ajouter des vérifications comme quoi tous les size sont positives
 */
HOG::HOG(unsigned char* image, int row_size, int col_size, SVM* svm)
{
	construct(image, row_size, col_size, svm);
}

HOG::HOG(Image* img, SVM* svm)
{
	construct(img->buffer,img->size_y,img->size_x,svm);
}

void HOG::construct(unsigned char* image, int row_size, int col_size, SVM* svm)
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
	xGrad = (signed short*)malloc(img_size*sizeof(short));
	yGrad = (signed short*)malloc(img_size*sizeof(short));
	norm = (signed int*)malloc(img_size*sizeof(int));
	angle = (signed int*)malloc(img_size*sizeof(int));
	detection = (bool*)malloc(win_size*sizeof(bool));

	cells = new Histogram[cell_size];
	blocks = new NormalizedHistogram[block_size];
	windows = new HogWindow[win_size];
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
		index = i*col_size;



		for (j=0;j<col_size;j++, index++)
		{
			//index = i*col_size+j;

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

			//signed int gradXe = xGrad[index];
			//signed int gradYe = yGrad[index];

			signed int n,a;
			signed int x_in=(signed int)xGrad[index];
			signed int y_in=(signed int)yGrad[index];
#ifdef TIE
			atan_cordic(y_in,x_in, &n, &a, 10);

#ifdef CHECK4ERRORS
			if (n<0 || n>CORDIC_SQRT2_255) {
				printf("Error: polar gradient norm out of bound\n");
			}
			if (a<0 || a>CORDIC_PI) {
				printf("Error: polar gradient angle out of bound\n");
			}
#endif
			norm[index] = (n>>CORDIC_FRAC_PART);
			angle[index] = a;
#else
			float x = ((float)x_in) /pow(2, CORDIC_FRAC_PART);
			float y = ((float)y_in) /pow(2, CORDIC_FRAC_PART);
			float slowNorm = sqrt(x*x+y*y);
			float slowAngle = atan2(y,x);
			if(slowAngle < 0) slowAngle += 3.1415926535;
			norm[index] = (int)(slowNorm * pow(2, CORDIC_FRAC_PART));
		    angle[index] = (int)(slowAngle * pow(2, CORDIC_FRAC_PART));
			//atan_cordic(y_in,x_in, &n, &a, 10);
#endif

			//float angle_cordicf = ((float)a)*180.0f/(float)CORDIC_PI;
			//float norm_cordicf = (float)n/(float)CORDIC_ONE;

			/*
			unsigned long xGradPow = ((unsigned long)xGrad[index])*xGrad[index];
			unsigned long yGradPow = ((unsigned long)yGrad[index])*yGrad[index];
			signed long gradDiv = (((signed long)yGrad[index])<<8)/((signed long)xGrad[index]+EPSILLON_INT);

			float norm_f = sqrt(xGradPow+yGradPow);
			float angle_f = (atan(gradDiv)*180/PI);
			float banane=24;
			*/
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
			blockIndex=winBlockIndex;
			//For each block of the window in y
			for (m=0;m<15;m++)
			{
				//For each block of the window in x
				for (n=0;n<7;n++, blockIndex++ )
				{
					//the index of the block in the global image block indexing system
					//blockIndex=winBlockIndex+(m*block_size_x)+n;

					//add the block to the current window's block list
					nhlist->push_back(&blocks[blockIndex]);
				}

				blockIndex += block_size_x-n;
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
	unsigned int detect=0;

	for (unsigned int i=0;i<win_size_y;i++)
	{
		for (unsigned int j=0;j<win_size_x;j++)
		{
			if (detection[(i*win_size_x)+j])
			{
				detect++;
				printf("Person detected in window x=%d,y=%d\n",j*8,i*8);
			}
		}
	}

	printf("%d detection on %d windows\n",detect,win_size);
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
		hist[i]=0;
	}
}

void Histogram::calculate_hist(const signed int* norm, const signed int* angle,int col_size)
{
	int i,j,index;
	char bin;
	signed int a,n;
	signed int excentricity, angleToNextBin, angleToCurrentBin;



	for (i=0;i<8;i++)
	{
		index=i*col_size;
		for (j=0;j<8;j++, index++)
		{
			a = angle[index];
			n = norm[index];
#ifdef TIE
			bin = tie_getbin(a,0);
			excentricity = a-binsAngle[bin];
#else

			// simple decision tree to determine the bin
			if (a<=binsAngle[4]) {
				if (a<=binsAngle[2]) {
					if (a<=binsAngle[1]) {
						bin=0;
					}
					else {
						bin=1;
					}
				}
				else {
					if (a<=binsAngle[3]) {
						bin=2;
					}
					else {
						bin=3;
					}
				}
			}
			else {
				if (a<=binsAngle[6]) {
					if (a<=binsAngle[5]) {
						bin=4;
					}
					else {
						bin=5;
					}
				}
				else {
					if (a<=binsAngle[7]) {
						bin=6;
					}
					else {
						if (a<=binsAngle[8]) {
							bin=7;
						}
						else {
							bin=8;
						}
					}
				}
			}
			//excentricity is between 0 and 1
			excentricity = (a-binsAngle[bin]);
#endif

#ifdef CHECK4ERRORS
			if (excentricity<0 || excentricity>CORDIC_NINTH_PI) {
				printf("Error: histogram excentricity is out of bound\n");
			}
#endif

			angleToNextBin = (n*excentricity)/CORDIC_NINTH_PI;

			//if the angle is above 160, the next bin is 0 degrees
			hist[(bin+1)%9]+= angleToNextBin;

			angleToCurrentBin = n-angleToNextBin;

#ifdef CHECK4ERRORS
			if (angleToCurrentBin<0) {
				printf("Error: Norm to current bin is out of bound\n");
			}
#endif

			hist[bin]+=angleToCurrentBin;
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
	l2hys = 0;

	for (int i=0;i<36;i++)
	{
		hist[i]=0;
	}
}

void NormalizedHistogram::calculate_normedhist(std::list<const Histogram*> hists)
{
	int i,j, index;

	//need a 64 bits container to store squared values
	signed int l2hysSum=0;

	index=0;
	for (std::list<const Histogram*>::iterator it=hists.begin(); it != hists.end(); it++)
	{
		for (j=0;j<9;j++)
		{
			//We will not right shift the result by CORDIC_FRAC_PART since the square root
			//will remove the fractional part added by the square.
			l2hysSum += ((*it)->hist[j])*((*it)->hist[j]);

			//debug remove later
			//this->hist[index] = ((*it)->hist[j]);
			//index++;
		}
	}

	l2hysSum = sqrt(l2hysSum);

	//Add an epsillon to avoid dividing by zero. This epsillon will be equal
	//to 1/2^CORDIC_FRAC_PART which is 0.000976562 for 10 bits
	l2hys = ((int)l2hysSum)+EPSILLON_INT;

	index=0;
	for (std::list<const Histogram*>::iterator it=hists.begin(); it != hists.end(); it++)
	{
		for (j=0;j<9;j++)
		{
			//left shift by CORDIC_FRAC_PART before dividing to preserve alignment.
			this->hist[index] = (((*it)->hist[j])<<CORDIC_FRAC_PART)/l2hys;
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
			for(unsigned int j=0;j<36;j++, index++)
			{
				values[index]=(*it)->hist[j];
			}
		}
	}
	else
	{
#ifdef CHECK4ERRORS
		printf("HogWindow trying to calculate values of a window with %d normalized histograms (105 required)",nhists.size());
#endif
	}
}

bool HogWindow::detect(const SVM* reference)
{
	unsigned int i;

	signed int dotProduct=0;

#ifdef TIE
	WUR_reg_A(dotProduct);

	for (i=0;i<3780;i+=2)
	{
		tie_add(this->values[i],reference->values[i],this->values[i+1],reference->values[i+1]);
	}
	dotProduct = RUR_reg_A();
#else
	for (i=0;i<3780;i++)
		{
			dotProduct += this->values[i]*reference->values[i];
		}
#endif

	dotProduct = dotProduct>>(CORDIC_FRAC_PART);
#ifdef CHECK4ERRORS
	printf("prod=%d \t reference=%d\n",dotProduct,reference->bias);
#endif
	//return (dotProduct<=-0.7*reference->bias);
	return (dotProduct<=-(reference->bias/10));
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
#ifdef CHECK4ERRORS
	if(fd==NULL){
		printf("Cannot open %s\n",filepath);
		return;
	}
#endif

	//Reading the file
	fread(&this->bias,sizeof(int),1,fd);
	fread(this->values,sizeof(int),3780,fd);

	fclose(fd);
}

