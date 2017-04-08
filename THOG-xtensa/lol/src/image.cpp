#include <stdio.h>
#include <stdlib.h>
#include "image.hpp"

Image::Image(std::string filepath)
{

    FILE* fd;
    int R_size, C_size;

    fd=fopen( filepath.c_str(), "rb" );

    if(fd==NULL){
    	return;
    }

    //Reading the image size
    fread(&R_size,sizeof(int),1,fd);
    fread(&C_size,sizeof(int),1,fd);

    size_img=R_size*C_size;

    buffer = (unsigned char*)malloc(size_img*sizeof(char));

    fread(buffer,1,size_img,fd);

    size_y=R_size;
    size_x=C_size;

    fclose(fd);
}

Image::~Image()
{
	free(buffer);
}
