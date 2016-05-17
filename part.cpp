#include "part.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void Part::accel() {
	ax[0] = -(exp(-0.5*cx2[0] -20.5*cx2[1])*cx[0]) - 41*exp(-20.5*cx2[0] - 0.5*cx2[1])*cx[0] + (200*cx[0])*exp(-50*(cx2[0] + cx2[1]));
	ax[1] = -41*exp(-0.5*cx2[0] -20.5*cx2[1])*cx[1] -exp(-20.5*cx2[0] - 0.5*cx2[1])*cx[1] + (200*cx[1])*exp(-50.0*(cx2[0] + cx2[1])); 
}
double Part::energy() {
	double ke = 0.5*(vx[0]*vx[0] + vx[1]*vx[1]);
	double pe = 1.0 - exp(-0.5*cx2[0] - 20.5*cx2[1]) - exp(-20.5*cx2[0] - 0.5*cx2[1]) + 2.0*exp(-50.0*(cx2[0] + cx2[1]));
	return ke + pe;
}

Part::Part(double x0, double x1, double vx0, double vx1) {
	//init values
	cx[0] = x0;
	cx[1] = x1;
	vx[0] = vx0;
	vx[1] = vx1;
	cx2[0] = cx[0] * cx[0];
	cx2[1] = cx[1] * cx[1];
	accel();

	//set position for iteration 1
	nx[0] = cx[0] + vx[0]*dt + 0.5*ax[0]*dt2;
	nx[1] = cx[1] + vx[1]*dt + 0.5*ax[1]*dt2;

	//set velocity for iteration 1
	vx[0] = (nx[0] - cx[0])/dt;
	vx[1] = (nx[1] - cx[1])/dt;
}
bool Part::reset(double x1, double vx1) {
	double tnx[2],tcx[2],tpx[2],tcx2[2],tvx[0];
	//save state
	tcx[0] = cx[0]; tcx[1] = cx[1];
	tpx[0] = px[0]; tpx[1] = px[1];
	tnx[0] = nx[0]; tnx[1] = nx[1];
	tvx[0] = vx[0]; tvx[1] = vx[1];

	//init values
	cx[0] = 0;
	cx[1] = x1;
	vx[0] = 0;
	vx[1] = vx1;
	cx2[0] = 0;
	cx2[1] = cx[1] * cx[1];
	accel();

	//initalize vx0
	vx[0] = -0.0725554 - energy();
	if(vx[0] > 0 ) {
		vx[0] = sqrt(2*vx[0]);

		//set position for iteration 1
		nx[0] = cx[0] + vx[0]*dt + 0.5*ax[0]*dt2;
		nx[1] = cx[1] + vx[1]*dt + 0.5*ax[1]*dt2;

		//set velocity for iteration 1
		vx[0] = (nx[0] - cx[0])/dt;
		vx[1] = (nx[1] - cx[1])/dt;
		return true;
	}
	else {
		cx[0] = tcx[0]; cx[1] = tcx[1];
		px[0] = tpx[0]; px[1] = tpx[1];
		nx[0] = tnx[0]; nx[1] = tnx[1];
		vx[0] = tvx[0]; vx[1] = tvx[1];
		return false;
	}
}
void Part::update() {
	//deccrement positions
	std::copy(cx,cx+2,px);
	std::copy(nx,nx+2,cx);
	cx2[0] = cx[0] * cx[0];
	cx2[1] = cx[1] * cx[1];

	//computer accelerations 
	accel();

	//update positions
	nx[0] = 2.0*cx[0] - px[0] + ax[0]*dt2;
	nx[1] = 2.0*cx[1] - px[1] + ax[1]*dt2;

	//update velocities
	vx[0] = (nx[0] - cx[0])/dt;
	vx[1] = (nx[1] - cx[1])/dt;
}
bool Part::check() {
	if( nx[0]*cx[0] < 0.00) {
		//std::cout << vx[1] << "\t" << nx[1] << std::endl;
		return true;
	}
	return false;
}
double Part::pos(int i) {
	return nx[i];
}
double Part::vel(int i) {
	return vx[i];
}
double Part::time(int i) {
	return dt * i;
}
