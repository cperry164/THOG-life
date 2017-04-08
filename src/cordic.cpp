#include "cordic.hpp"

#include "C:\TEMP\THOG-xtensa\XtensaInfo\Models\tie_dev1.tdk\include\xtensa\tie\first_tie.h"

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

	tie_cordic_init(x,y);

	for (k=0; k<n; ++k)
	{
		//direction
		d = ((y<=0) ? 0 : -1);

		//new values
		tx = x - (((y>>k) ^ d) - d);
		ty = y + (((x>>k) ^ d) - d);
		tz = z - ((cordic_ctab[k] ^ d) - d);

		tie_cordic_iterate(4000,-2000);

		signed int tie_x = RUR_cordic_x();
		signed int tie_y = RUR_cordic_y();
		signed int tie_z = RUR_cordic_z();
		int mask = RUR_mask_out();
		signed int xdiv2 = RUR_xdiv2_out();
		signed int ydiv2 = RUR_ydiv2_out();
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
