#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>

#include "Body.h"

#include "Runge-Kutta-4.h"
#include "Embedded-Runge-Kutta.h"
#include "Verlet.h"
#include "Forest-Ruth.h"
#include "PEFRL.h"

using namespace std;

// functie die de positie en snelheid van het massamiddelpunt berekent
// en deze naar nul zet door te corrigeren in de posities en snelheden van de lichamen
void vastCOM(vector<Body>* bodies) {
	Vec COM_pos = Vec();
	Vec COM_vel = Vec();
	double Mass = 0;
	for (unsigned int i = 0; i < bodies->size(); i++) {
		COM_pos += (*bodies)[i].getpos() * (*bodies)[i].getmass();
		COM_vel += (*bodies)[i].getvel() * (*bodies)[i].getmass();
		Mass += (*bodies)[i].getmass();
	}
	COM_pos /= Mass;
	COM_vel /= Mass;
	for (unsigned int i = 0; i < bodies->size(); i++) {
		(*bodies)[i].setpos((*bodies)[i].getpos() - COM_pos);
		(*bodies)[i].setvel((*bodies)[i].getvel() - COM_vel);
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

	// zorgen dat het COM niet beweegt
	vastCOM(&bodies);
	// vastCOM(&bodies);

	// de verschillende integratoren:
	time_t start, eind;
	double tijd;

	start = time(NULL);
	RK4_1(bodies, h, tstart, teind);
	eind = time(NULL);
	tijd = difftime(eind, start);
	cout << "tijd RK41: " << tijd << endl;

	start = time(NULL);
	RK4_2(bodies, h, tstart, teind);
	eind = time(NULL);
	tijd = difftime(eind, start);
	cout << "tijd RK42: " << tijd << endl;

	start = time(NULL);
	ERK(bodies, h, tstart, teind);
	eind = time(NULL);
	tijd = difftime(eind, start);
	cout << "tijd ERK: " << tijd << endl;

	//Verlet(bodies, h, tstart, teind);
	//FR(bodies, h, tstart, teind);
	//PEFRL(bodies, h, tstart, teind);

	cout << "finished simulating, enter key to stop the program..." << endl;
	getchar();
}
