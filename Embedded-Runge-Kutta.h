#ifndef ERK_H
#define ERK_H

#include <iostream>
#include "Body.h"
#include <vector>
#include <fstream>
using namespace std;

// gegeven de punten op hun beginposities, uitvoeren van de Embedded RK methode
void ERK(vector<Body> bodies, double h, double tstart, double teind) {
	vector<double> mass;
	double energy = calc_ener(bodies);

	// vector van alle posities en snelheden
	vector<Vec> yn;
	for (unsigned int i = 0; i < bodies.size(); i++) {
		yn.push_back(bodies[i].getpos());
		yn.push_back(bodies[i].getvel());
		mass.push_back(bodies[i].getmass());
	}

	// definiëren van verschillende objecten nodig tijdens het integratieschema
	vector<Vec> k1;
	vector<Vec> k2;
	vector<Vec> k3;
	vector<Vec> k4;
	vector<Vec> k5;
	vector<Vec> k6;
	vector<Vec> yn1;
	vector<Vec> yn1ster;

	// aanmaken van de tekstbestanden waar de data naartoe wordt geschreven
	ofstream posities("posities_ERK.txt");
	ofstream snelheden("snelheden_ERK.txt");
	ofstream ener("energie_ERK.txt");

	// het integratieschema uitvoeren
	for (double t = tstart; t < teind; t += h) {
		// berekenen van de relevante formules
		k1 = calc_k(yn, mass, h);
		k2 = calc_k(yn + 0.25 * k1, mass, h);
		k3 = calc_k(yn + (3. / 32.)*k1 + (9. / 32.)*k2, mass, h);
		k4 = calc_k(yn + (1932. / 2197.)*k1 - (7200. / 2197.)*k2 + (7296. / 2197.)*k3, mass, h);
		k5 = calc_k(yn + (439. / 216.)*k1 - 8 * k2 + (3680. / 513.)*k3 - (845. / 4104.)*k4, mass, h);
		k6 = calc_k(yn - (8. / 27.)*k1 + 2 * k2 - (3544. / 2565.)*k3 + (1859. / 4104.)*k4 - (11. / 40.)*k5, mass, h);

		yn1 = yn + (16. / 135.)*k1 + (6656. / 12825.)*k3 + (28561. / 56430.)*k4 - (9. / 50.)*k5 + (2. / 55.)*k6;
		yn1ster = yn + (25. / 216.)*k1 + (1408. / 2565.)*k3 + (2197. / 4104.)*k4 - (1. / 5.)*k5;

		// de posities en snelheden van de body's bijwerken
		for (unsigned int i = 0; i < bodies.size(); i++) {
			bodies[i].setpos(yn1[2 * i]);
			bodies[i].setvel(yn1[2 * i + 1]);
		}
		yn = yn1;

		// de posities, snelheden en de relatieve energiefout uitschrijven naar tekstbestanden
		posities << t << ' ';
		snelheden << t << ' ';

		for (unsigned int i = 0; i < bodies.size(); i++) {
			posities << ' ' << bodies[i].getpos().getx() << ' ' << bodies[i].getpos().gety() << ' ' << bodies[i].getpos().getz();
			snelheden << ' ' << bodies[i].getvel().getx() << ' ' << bodies[i].getvel().gety() << ' ' << bodies[i].getvel().getz();
		}
		posities << endl;
		snelheden << endl;

		ener << t << ' ' << (calc_ener(bodies) - energy) / energy << endl;
	}
}

#endif ERK_H
