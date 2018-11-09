#ifndef RK4_H
#define RK4_H

#include <iostream>
#include "Body.h"
#include <vector>
#include <fstream>
using namespace std;

// functie die gegeven de huidige toestand van het systeem, initiële energie en de tijd
// de data in het correcte format output: (t, (x, y, z, vx, vy, vz) x N, e, relatieve-energiefout)
void uitschrijven_data(const double& t, const vector<Body>& bodies, const double& e0,  ofstream& output_file) {
  output_file << t << ' ';
  for (Body bod : bodies) {
    output_file << bod.getpos().getx() << ' ' << bod.getpos().gety() << ' ' << bod.getpos().getz();
    output_file << ' ' << bod.getvel().getx() << ' ' << bod.getvel().gety() << ' ' << bod.getvel().getz() << ' ';
  }
  double e = calc_ener(bodies);
  output_file << e << ' ' << abs((e - e0) / e0) << endl;
}

// gegeven de punten op hun beginposities, uitvoeren van de RK4 methode
void RK4_1(vector<Body> bodies, const double& h, const double& tstart, const double& teind) {
  // definiëren van verschillende objecten nodig tijdens het integratieschema
	vector<Grad> k1;
	vector<Grad> k2;
	vector<Grad> k3;
	vector<Grad> k4;
  double t = tstart;
  //initiële energie opslaan
  const double e0 = calc_ener(bodies);
  // aanmaken van de tekstbestanden waar de data naartoe wordt geschreven
	ofstream output_file("output_RK4_1.txt");
  // initiële toestand uitschrijven
  uitschrijven_data(t, bodies, e0, output_file);
  // het integratieschema uitvoeren
	for (t += h; t < teind + h; t += h) {
    k1 = h * calc_grad(bodies);
    k2 = h * calc_grad(bodies + 0.5 * k1);
    k3 = h * calc_grad(bodies + 0.5 * k2);
    k4 = h * calc_grad(bodies + k3);
    bodies = bodies +  (1. / 6.) * k1 + (1. / 3.) * k2 + (1. / 3.) * k3 + (1. / 6.) * k4;
    // uitschrijven van de data
    uitschrijven_data(t, bodies, e0, output_file);
  }
  output_file.close();
}

// gegeven de punten op hun beginposities, uitvoeren van de RK4 methode
void RK4_2(vector<Body> bodies, double h, double tstart, double teind) {
	vector<double> mass;
	// vector van alle posities en snelheden
	vector<Vec> y;
	for (unsigned int i = 0; i < bodies.size(); i++) {
		y.push_back(bodies[i].getpos());
		y.push_back(bodies[i].getvel());
		mass.push_back(bodies[i].getmass());
	}

	// definiëren van verschillende objecten nodig tijdens het integratieschema
	vector<Vec> k1;
	vector<Vec> k2;
	vector<Vec> k3;
	vector<Vec> k4;
	vector<Vec> yn1;
  double t = tstart;

  //initiële energie opslaan
  const double e0 = calc_ener(bodies);

  // aanmaken van de tekstbestanden waar de data naartoe wordt geschreven
	ofstream output_file("output_RK4_2.txt");

  // initiële toestand uitschrijven
  uitschrijven_data(t, bodies, e0, output_file);

	// het integratieschema uitvoeren
	for (t += h ; t < teind + h; t += h) {
		// berekenen van de relevante formules
		k1 = calc_k(y, mass, h);
		k2 = calc_k(y + 0.5 * k1, mass, h);
		k3 = calc_k(y + 0.5 * k2, mass, h);
		k4 = calc_k(y + k3, mass, h);

		y = y + (1. / 6.) * k1 + (1. / 3.) * k2 + (1. / 3.) * k3 + (1. / 6.) * k4;

		// de posities en snelheden van de body's bijwerken
		for (unsigned int i = 0; i < bodies.size(); i++) {
			bodies[i].setpos(y[2 * i]);
			bodies[i].setvel(y[2 * i + 1]);
		}

		// de posities, snelheden en de relatieve energiefout uitschrijven naar tekstbestanden
		uitschrijven_data(t, bodies, e0, output_file);
	}
  output_file.close();
}

#endif RK4_H
