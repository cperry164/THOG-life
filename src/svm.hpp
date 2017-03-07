#ifndef __SVM_H_
#define __SVM_H_

#include <list>

class SVM
{
public:
	SVM();

	bool detect(SVM* reference);
private:
	//array of float of n dimensions. HOG uses 3780 dimensions
	float* values;
};

#endif
