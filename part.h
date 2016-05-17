#pragma once 

class Part{
	const double dt = 0.0001;
	const double dt2 = dt*dt;
	const double idt = 1.0/dt;
	double px[2], cx[2], nx[2];
	double cx2[2];
	double vx[2], ax[2];
	void accel();
public:
	Part(double, double, double, double);
	bool reset(double,double);
	void update();
	bool check();
	double energy();
	double pos(int);
	double vel(int);
	double time(int);
};
