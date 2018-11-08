#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Body.h"
#include "Runge-Kutta-4.h"
#include "Embedded-Runge-Kutta.h"
#include "Verlet.h"
#include "Forest-Ruth.h"
#include "PEFRL.h"
using namespace std;

// functie die de snelheid van het massamiddelpunt berekent en dit naar nul zet door te corrigeren in de snelheden van de lichamen
void vastCOM(vector<Body>* bodies) {
	unsigned int aantal = bodies->size();
	Vec COMpos = Vec();
	Vec COMvel = Vec();
	double Mass = 0;

	for (unsigned int i = 0; i < aantal; i++) {
		COMpos += (*bodies)[i].getpos() * (*bodies)[i].getmass();
		COMvel += (*bodies)[i].getvel() * (*bodies)[i].getmass();
		Mass += (*bodies)[i].getmass();
	}

	COMpos /= Mass;
	COMvel /= Mass;

	for (unsigned int i = 0; i < aantal; i++) {
		(*bodies)[i].setpos((*bodies)[i].getpos() - COMpos);
		(*bodies)[i].setvel((*bodies)[i].getvel() - COMvel);
		(*bodies)[i].setmass((*bodies)[i].getmass() / Mass);
	}
}

// functie die de posities en snelheden herschaalt zodat is voldaan aan het viriaal theorema
void scaling(vector<Body>* bodies) {
	unsigned int aantal = bodies->size();
	double Epot = 0;
	double Ekin = 0;

	for (unsigned int i = 0; i < aantal; i++) {
		Ekin += 0.5 * (*bodies)[i].getmass() * (*bodies)[i].getvel().norm2();
		for (unsigned int j = 0; j < aantal; j++) {
			if (i != j) {
				Epot -= 0.5 * (*bodies)[i].getmass() * (*bodies)[j].getmass() / ((*bodies)[i].getpos() - (*bodies)[j].getpos()).norm();
			}
		}
	}

	for (unsigned int i = 0; i < aantal; i++) {
		(*bodies)[i].setpos((*bodies)[i].getpos() * (-2) * Epot);
		(*bodies)[i].setvel((*bodies)[i].getvel() * 0.5 / pow(Ekin, 0.5));
	}
}

// the main function
int main()
{
	// bepalen tijdsstap en tijdsinterval
	double h = 0.01;
	double tstart = 0.;
	double teind = 1000.;

	// op basis van een bestand met beginwaarden de vector van body's opstellen voor de simulaties
	vector<Body> bodies;

	ifstream parameters ("beginwaarden.txt");
	int aantal;
	double rx, ry, rz, vx, vy, vz, m;

	// eerste waarde van het bestand geeft het aantal body's
	parameters >> aantal;
	for (int i = 0; i < aantal; i++) {

		// voor elke body de positie, snelheid en massa (elke component op een andere lijn -> 7 lijnen per body)
		parameters >> rx;
		parameters >> ry;
		parameters >> rz;

		parameters >> vx;
		parameters >> vy;
		parameters >> vz;

		parameters >> m;

		// body aanmaken en toevoegen aan de vector
		Body lichaam = Body(rx, ry, rz, vx, vy, vz, m);
		bodies.push_back(lichaam);
	}
	parameters.close();

	// zorgen dat het COM niet beweegt, in de oorsprong staat, en totale massa gelijk aan 1
	vastCOM(&bodies);

	// unit scaling --> zorgen dat de totale energie = -1/4
	scaling(&bodies);

	// de verschillende integratoren:
	RK4(bodies, h, tstart, teind);
	ERK(bodies, h, tstart, teind);
	Verlet(bodies, h, tstart, teind);
	FR(bodies, h, tstart, teind);
	PEFRL(bodies, h, tstart, teind);

	cout << "finished simulating, enter key to stop the program..." << endl;
	getchar();
}
