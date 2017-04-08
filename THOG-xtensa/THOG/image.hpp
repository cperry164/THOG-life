#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <string>

class Image
{
public:
	Image(std::string filepath);
	~Image();

	int size_x, size_y, size_img;
	char *buffer;
};


#endif
