#include "cordic.hpp"


//Taken online from http://www.dcs.gla.ac.uk/~jhw/cordic/

void cordic(int theta, int *s, int *c, int n)
{
  int k, d, tx, ty, tz;
  int x=cordic_1K,y=0,z=theta;
  n = (n>CORDIC_NTAB) ? CORDIC_NTAB : n;
  for (k=0; k<n; ++k)
  {
    d = z>>31;
    //get sign. for other architectures, you might want to use the more portable version
    //d = z>=0 ? 0 : -1;
    tx = x - (((y>>k) ^ d) - d);
    ty = y + (((x>>k) ^ d) - d);
    tz = z - ((cordic_ctab[k] ^ d) - d);
    x = tx; y = ty; z = tz;
  }
 *c = x; *s = y;
}

void atan_cordic(signed int y_in, signed int x_in, signed int *norm, signed int *angle, signed int n)
{
	unsigned int k;
	signed int d, tx, ty, tz;
	signed int x= (x_in>=0) ? cordic_1K*x_in : -cordic_1K*x_in;
	signed int y= (y_in>=0) ? cordic_1K*y_in : -cordic_1K*y_in;
	signed int z=0;

	//nombre d'itérations maximums
	n = (n>CORDIC_NTAB) ? CORDIC_NTAB : n;

	//special case with y=0
	if (y_in==0)
	{
		if (x_in>=0) {
			*norm = x_in;
			*angle = 0;
		}
		else {
			*norm = -x_in;
			*angle = CORDIC_PI;
		}
		return;
	}

	for (k=0; k<n; ++k)
	{
		//direction
		d = ((y<=0) ? 0 : -1);

		//new values
		tx = x - (((y>>k) ^ d) - d);
		ty = y + (((x>>k) ^ d) - d);
		tz = z - ((cordic_ctab[k] ^ d) - d);

		//update
		x = tx;
		y = ty;
		z = tz;
	}

	//set outputs
	*norm = x;

	//quadrant 1
	if (x_in>=0 && y_in>=0) {
		*angle = z;
	}
	//quadrant 2
	else if (x_in>=0 && y_in<0) {
		*angle = CORDIC_PI-z;
	}
	//quadrant 3
	else if (x_in<0 && y_in>=0) {
		*angle = CORDIC_PI-z;
	}
	//quadrant 4
	else {
		*angle = z;
	}

}
