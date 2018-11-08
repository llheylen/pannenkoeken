#ifndef BODY_H
#define BODY_H

#include <cmath>
#include <vector>

double G = 1;

class Vec {
private:
	double x;
	double y;
	double z;
public:
	Vec() { x = 0; y = 0; z = 0; }
	Vec(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }

	void setx(double x) { this->x = x; }
	void sety(double y) { this->y = y; }
	void setz(double z) { this->z = z; }

	double getx() { return x; }
	double gety() { return y; }
	double getz() { return z; }

	double norm() const { return sqrt(x*x + y*y + z*z); }
	double norm2() const { return x*x + y*y + z*z; }
	double norm3() const { double r = sqrt(x*x + y*y + z*z); return r*r*r; }

	Vec& operator+=(Vec v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vec& operator-=(Vec v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Vec& operator*=(double s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	Vec& operator/=(double s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
};

// operations between two vectors
Vec operator+(Vec a, Vec b) { return a += b; }
Vec operator-(Vec a, Vec b) { return a -= b; }
// operations between a vector and a scalar
Vec operator*(Vec a, double s) { return a *= s; }
Vec operator*(double s, Vec b) { return b *= s; }
Vec operator/(Vec a, double s) { return a /= s; }


class Body {
private:
	Vec position;
	Vec velocity;
	double mass;
public:
	// basisconstructor: stilstaand deeltje in de oorsprong met massa = 1
	Body() { position = Vec(); velocity = Vec(); mass = 1; }
	// constructor waarin alle componenten van de positie en de snelheid, en de massa worden meegegeven
	Body(double rx, double ry, double rz, double vx, double vy, double vz, double m) {
		position = Vec(rx, ry, rz);
		velocity = Vec(vx, vy, vz);
		mass = m;
	}

	void setpos(Vec pos) {
		position.setx(pos.getx());
		position.sety(pos.gety());
		position.setz(pos.getz());
	}
	void setvel(Vec vel) {
		velocity.setx(vel.getx());
		velocity.sety(vel.gety());
		velocity.setz(vel.getz());
	}
	void setmass(double M) { mass = M; }

	Vec getpos() { return position; }
	Vec getvel() { return velocity; }
	double getmass() { return mass; }
};

// operatoren om 2 vectoren op te tellen en af te trekken
std::vector<Vec> operator+(std::vector<Vec> a, std::vector<Vec> b) {
	std::vector<Vec> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] + b[i]);
	}
	return c;
}
std::vector<Vec> operator-(std::vector<Vec> a, std::vector<Vec> b) {
	std::vector<Vec> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] - b[i]);
	}
	return c;
}

// operator om een vector te vermenigvuldigen met een getal
std::vector<Vec> operator*(double a, std::vector<Vec> b) {
	std::vector<Vec> c;
	for (unsigned int i = 0; i < b.size(); i++) {
		c.push_back(a*b[i]);
	}
	return c;
}

// gegeven een lijst (vector) van body's, geeft de versnelling terug die elke body ondervindt
std::vector<Vec> calc_accel(std::vector<Body> points) {
	// initialiseren van een lijst met N lege versnellingsvectoren
	std::vector<Vec> accel;

	// alle body's overlopen en de versnelling berekenen door te itereren over de andere body's
	for (unsigned int i = 0; i < points.size(); i++) {
		Vec a = Vec();
		for (unsigned int j = 0; j < points.size(); j++) {
			if (i != j) {
				a -= G * points[j].getmass() * (points[i].getpos() - points[j].getpos()) / (points[i].getpos() - points[j].getpos()).norm3();
			}
		}
		accel.push_back(a);
	}
	return accel;
}

// berekenen van de verschillende k's voor de Runge-Kutta methoden
std::vector<Vec> calc_k(std::vector<Vec> y, std::vector<double> mass, double h) {
	std::vector<Vec> g;
	for (unsigned int i = 0; i < mass.size(); i++) {
		// voor de posities is g gewoon de snelheid
		g.push_back(y[2 * i + 1]);

		// voor de snelheden wordt dit de versnelling
		Vec a = Vec();
		for (unsigned int j = 0; j < mass.size(); j++) {
			if (i != j) {
				a -= G * mass[j] * (y[2*i] - y[2*j]) / (y[2*i] - y[2*j]).norm3();
			}
		}
		g.push_back(a);
	}
	return h*g;
}

// berekenen van de totale energie gegeven een lijst van alle body's
double calc_ener(std::vector<Body> points) {
	double ener = 0;

	for (unsigned int i = 0; i < points.size(); i++) {
		ener += 0.5 * points[i].getmass() * points[i].getvel().norm2();

		for (unsigned int j = 0; j < points.size(); j++) {
			if (i != j) {
				ener -= 0.5 * G * points[i].getmass() * points[j].getmass() / (points[i].getpos() - points[j].getpos()).norm();
			}
		}
	}

	return ener;
}

#endif BODY_H