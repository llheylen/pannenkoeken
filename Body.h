#ifndef BODY_H
#define BODY_H

#include <cmath>
#include <vector>

double G = 1.;

// klasse die vectoren in 3D voorstelt
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

	double norm() const { return sqrt(x*x + y * y + z * z); }
	double norm2() const { return x * x + y * y + z * z; }
	double norm3() const { double r = sqrt(x*x + y * y + z * z); return r * r*r; }

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


// klasse die de gradiënt van een body in de faseruimte voorstelt
// (basicly gewoon een vector met 6 componenten)
class Grad {
private:
	Vec position_grad;
	Vec velocity_grad;
public:
	// basisconstructor: nulgradiënt
	Grad() { position_grad = Vec(); velocity_grad = Vec(); }
	// constructor waarin positie- en snelheidsgradiënt worden meegegeven
	Grad(Vec pos_grad, Vec vel_grad) {
		position_grad = pos_grad;
		velocity_grad = vel_grad;
	}

	void setpos_grad(Vec pos_grad) { position_grad = pos_grad; }
	void setvel_grad(Vec vel_grad) { velocity_grad = vel_grad; }

	Vec getpos_grad() { return position_grad; }
	Vec getvel_grad() { return velocity_grad; }

	Grad& operator+=(Grad g) {
		position_grad += g.position_grad;
		velocity_grad += g.velocity_grad;
		return *this;
	}
	Grad& operator-=(Grad g) {
		position_grad -= g.position_grad;
		velocity_grad -= g.velocity_grad;
		return *this;
	}
	Grad& operator*=(double s) {
		position_grad *= s;
		velocity_grad *= s;
		return *this;
	}
	Grad& operator/=(double s) {
		position_grad /= s;
		velocity_grad /= s;
		return *this;
	}
};

// operations between two gradients
Grad operator+(Grad a, Grad b) { return a += b; }
Grad operator-(Grad a, Grad b) { return a -= b; }
// operations between a gradient and a scalar
Grad operator*(Grad a, double s) { return a *= s; }
Grad operator*(double s, Grad b) { return b *= s; }
Grad operator/(Grad a, double s) { return a /= s; }

// klasse die de ogenblikkelijke toestand van een lichaam voorselt
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

	void setpos(Vec pos) { position = pos; }
	void setvel(Vec vel) { velocity = vel; }
	void setmass(double M) { mass = M; }

	Vec getpos() { return position; }
	Vec getvel() { return velocity; }
	double getmass() { return mass; }

	// bewerkingen om een gradiënt bij een body op te tellen: evolutie in de faseruimte
	Body& operator+=(Grad g) {
		position += g.getpos_grad();
		velocity += g.getvel_grad();
		return *this;
	}

	Body& operator-=(Grad g) {
		position -= g.getpos_grad();
		velocity -= g.getvel_grad();
		return *this;
	}

};

// bewerkingen om een gradiënt bij een body op te tellen: evolutie in de faaseruimte
Body operator+(Body b, Grad g) { return b += g; }
Body operator+(Grad g, Body b) { return b += g; }
Body operator-(Body b, Grad g) { return b -= g; }

// nu een heleboel operatoren om bewerkingen op vectoren van de klassen hierboven uit te voeren

// operatoren om 2 vectoren met als elementen Vec op te tellen en af te trekken
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

// operator om een vector met als elementen Vec te vermenigvuldigen met een getal
std::vector<Vec> operator*(double a, std::vector<Vec> b) {
	std::vector<Vec> c;
	for (unsigned int i = 0; i < b.size(); i++) {
		c.push_back(a*b[i]);
	}
	return c;
}

std::vector<Vec> operator*(std::vector<Vec> b, double a) {
	std::vector<Vec> c;
	for (unsigned int i = 0; i < b.size(); i++) {
		c.push_back(a*b[i]);
	}
	return c;
}

// operatoren om 2 vectoren met als elementen Grad op te tellen en af te trekken
std::vector<Grad> operator+(std::vector<Grad> a, std::vector<Grad> b) {
	std::vector<Grad> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] + b[i]);
	}
	return c;
}

std::vector<Grad> operator-(std::vector<Grad> a, std::vector<Grad> b) {
	std::vector<Grad> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] - b[i]);
	}
	return c;
}

// operatoren om een vector met als elementen Grad te vermenigvuldigen met een getal
std::vector<Grad> operator*(double a, std::vector<Grad> b) {
	std::vector<Grad> c;
	for (unsigned int i = 0; i < b.size(); i++) {
		c.push_back(a*b[i]);
	}
	return c;
}

std::vector<Grad> operator*(std::vector<Grad> b, double a) {
	std::vector<Grad> c;
	for (unsigned int i = 0; i < b.size(); i++) {
		c.push_back(a*b[i]);
	}
	return c;
}

// operatoren om 2 vectoren met als elementen Body en Grad op te tellen en af te trekken
std::vector<Body> operator+(std::vector<Body> a, std::vector<Grad> b) {
	std::vector<Body> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] + b[i]);
	}
	return c;
}

std::vector<Body> operator-(std::vector<Body> a, std::vector<Grad> b) {
	std::vector<Body> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] - b[i]);
	}
	return c;
}

std::vector<Body> operator+(std::vector<Grad> b, std::vector<Body> a) {
	std::vector<Body> c;
	for (unsigned int i = 0; i < a.size(); i++) {
		c.push_back(a[i] + b[i]);
	}
	return c;
}

// en nu de fysica

// gegeven een lijst (vector) van body's, geeft de versnelling terug die elke body ondervindt
std::vector<Vec> calc_accel(std::vector<Body> points) {
	// initialiseren van een lijst met N lege versnellingsvectoren
	std::vector<Vec> accel;

	// alle body's overlopen en de versnelling berekenen door te itereren over de andere body's
	for (unsigned int i = 0; i < points.size(); i++) {
		Vec a = Vec();
		for (unsigned int j = 0; j < points.size(); j++) {
			if (i != j) {
				a -= points[j].getmass() * (points[i].getpos() - points[j].getpos()) / (points[i].getpos() - points[j].getpos()).norm3();
			}
		}
		a *= G;
		accel.push_back(a);
	}
	return accel;
}

// berekenen van de totale energie gegeven een lijst van alle body's
double calc_ener(std::vector<Body> points) {
	double ener_kin = 0;
	double ener_pot = 0;

	for (unsigned int i = 0; i < points.size(); i++) {
		ener_kin += points[i].getmass() * points[i].getvel().norm2();

		for (unsigned int j = 0; j < points.size(); j++) {
			if (i != j) {
				ener_pot -= points[i].getmass() * points[j].getmass() / (points[i].getpos() - points[j].getpos()).norm();
			}
		}
	}
	ener_kin *= 0.5;
	ener_pot *= 0.5 * G;
	return ener_kin + ener_pot;
}

// berekenen van de totale gradiënt (vector met als elementen de gradiënt van elk deeltje)
// van het systeem  gegeven de configuratie van het systeem (vector met als elementen de lichamen)
std::vector<Grad> calc_grad(std::vector<Body> bodies) {
	std::vector<Grad> g;
	std::vector<Vec> accelerations = calc_accel(bodies);
	for (unsigned int i = 0; i < bodies.size(); i++) {
		// voor de posities is g gewoon de snelheid
		g.push_back(Grad(bodies[i].getvel(), accelerations[i]));
	}
	return g;
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
				a -= mass[j] * (y[2 * i] - y[2 * j]) / (y[2 * i] - y[2 * j]).norm3();
			}
		}
		a *= G;
		g.push_back(a);
	}
	return h * g;
}

// functie die gegeven de huidige toestand van het systeem, initiële energie en de tijd
// de data in het correcte format output: (t, (x, y, z, vx, vy, vz) x N, e, relatieve-energiefout)
void uitschrijven_data(double t, std::vector<Body> bodies, double e0, std::ofstream& output_file) {
	output_file << t << ' ';
	for (Body bod : bodies) {
		output_file << bod.getpos().getx() << ' ' << bod.getpos().gety() << ' ' << bod.getpos().getz();
		output_file << ' ' << bod.getvel().getx() << ' ' << bod.getvel().gety() << ' ' << bod.getvel().getz() << ' ';
	}
	double e = calc_ener(bodies);
	output_file << e << ' ' << abs((e - e0) / e0) << std::endl;
}

#endif BODY_H
