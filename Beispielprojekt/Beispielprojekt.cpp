#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <random>
//Wozu time ?
#include <time.h>



using namespace std;

int Bildschirmbreite = 800;
int Bildschirmhöhre = 600;


//Hilfs Struct für die Position der Schlange
struct Position
{
public:
	int x;
	int y;
};

int bewegung_verlangsamen = 0;

class Schlange
{
public:
	int laenge = 10;
	int geschwindigkeit = 10;

	//Versuch, vielleicht sieht es so nach dem Essen besser aus
	bool draw_freigabe = true;

	string richtung = "left";

	//Vektor für Position der Schlange
	vector<Position> pos;

	//Bewegen der Schlange
	void move();

	//Verlängern der Schlange nach essen
	void verlaengern();

	//Konstruktor der Schlange
	Schlange()
	{
		this->laenge = 10;

		//Startwert festlegen
		Position p = { 400,300 };
		for (int i = 0; i < laenge; i++)
		{
			this->pos.push_back(p);
			p.x = p.x + 10;
		}
	}
};

//++++++++++++++++++++++++++++++++++++++++NEU++++++++++++++++++++++++++++++++++++
// Ich habe Position in public gemacht weil ich es einfacher fand - ist natürlich nicht optimal
class Essen
{
	int laenge;

public:

	Position posess;

	// random
	void random_posess();
	int random_x();
	int random_y();

	//Konstruktor des Essens
	Essen()
	{
		this->laenge = 1;

		//Startwert festlegen
		this->posess = { 10 * this->random_x(), 10 * this->random_y() };

	}
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


class GameWindow : public Gosu::Window
{
public:

	//Schlange erstellen
	Schlange s;

	// Essen erstellen
	Essen e;

	GameWindow()
		: Window(805, 605)//, true) noch anhängen, wenn Vollbild benötigt wird.
	{
		set_caption("Snake by Kevin-Marcel Schnell & Nils Hepp");
	}

	void draw() override
	{
		//Schlange zeichnen 
		if (s.draw_freigabe)
		{
			for (size_t i = 0; i < s.pos.size(); i++)
			{
				if (i == 0)
				{
					graphics().draw_quad(
						(s.pos.at(i).x - 5), (s.pos.at(i).y + 5), Gosu::Color::RED,
						(s.pos.at(i).x + 5), (s.pos.at(i).y + 5), Gosu::Color::RED,
						(s.pos.at(i).x - 5), (s.pos.at(i).y - 5), Gosu::Color::RED,
						(s.pos.at(i).x + 5), (s.pos.at(i).y - 5), Gosu::Color::RED,
						0.0);
				}
				else
				{
					graphics().draw_quad(
						(s.pos.at(i).x - 5), (s.pos.at(i).y + 5), Gosu::Color::GRAY,
						(s.pos.at(i).x + 5), (s.pos.at(i).y + 5), Gosu::Color::GRAY,
						(s.pos.at(i).x - 5), (s.pos.at(i).y - 5), Gosu::Color::GRAY,
						(s.pos.at(i).x + 5), (s.pos.at(i).y - 5), Gosu::Color::GRAY,
						0.0);
				}
			}
		}

		// Essen zeichnen ++++++++++++++++++++++++++++++++++++++++++++++

		graphics().draw_quad(
			(e.posess.x - 5), (e.posess.y + 5), Gosu::Color::GREEN,
			(e.posess.x + 5), (e.posess.y + 5), Gosu::Color::GREEN,
			(e.posess.x - 5), (e.posess.y - 5), Gosu::Color::GREEN,
			(e.posess.x + 5), (e.posess.y - 5), Gosu::Color::GREEN,
			0.0);
		
		/*graphics().draw_rect(
			e.posess.x, e.posess.y, 10, 10, Gosu::Color::GREEN, 0
		);*/

	}


	void update() override
	{
		//Einlesen der Bedientasten
		if ((Gosu::Input::down(Gosu::KB_LEFT) || Gosu::Input::down(Gosu::GP_LEFT)) && s.richtung != "right") {
			s.richtung = "left";
		}
		if ((Gosu::Input::down(Gosu::KB_RIGHT) || Gosu::Input::down(Gosu::GP_RIGHT)) && s.richtung != "left") {
			s.richtung = "right";
		}
		if ((Gosu::Input::down(Gosu::KB_UP) || Gosu::Input::down(Gosu::GP_LEFT)) && s.richtung != "down") {
			s.richtung = "up";
		}
		if ((Gosu::Input::down(Gosu::KB_DOWN) || Gosu::Input::down(Gosu::GP_RIGHT)) && s.richtung != "up") {
			s.richtung = "down";
		}

		// Neues Essen erscheinen lassen + Schlange länger machen ++++++++++++++++++++ !Funktioniert noch nicht!
		if ((s.pos.at(0).x == e.posess.x) && (s.pos.at(0).y == e.posess.y)) {
			s.geschwindigkeit--;
			e.random_posess();
			s.verlaengern();
		}

		//Geschwindigkeitsregelung für die Schlange (nicht Linear!!)
		if (bewegung_verlangsamen > s.geschwindigkeit)
		{
			s.move();
			bewegung_verlangsamen = 0;
		}
		bewegung_verlangsamen++;

		
		// Spielfeldbegrenzung ++++++++++++++++++++++++++++++++
		if (s.pos.at(0).x <= -5 || s.pos.at(0).x > 800 || s.pos.at(0).y <= -1 || s.pos.at(0).y > 600) {
		Window:close(); // Schließt nur, lässt nicht neu starten.
		}

		// Falls Schlange sich selbst essen will
		for (size_t i = 1; i < s.pos.size(); i++)
		{
			if ((s.pos.at(0).x == s.pos.at(i).x) && (s.pos.at(0).y == s.pos.at(i).y))
			{
				Window::close();
			}
		}

	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}

//Methoden

void Schlange::move()
{
	//Nachlaufen der hinteren Schlangenteile
	for (size_t i = 0; i < (this->pos.size() - 1); i++)
	{
		this->pos.at(this->pos.size() - 1 - i) = this->pos.at(this->pos.size() - i - 2);
	}

	//Navigieren des Schlangenkopfes 
	//Links
	if (this->richtung == "left")
	{
		this->pos.at(0).x = this->pos.at(0).x - 10;
	}
	//Rechts
	if (this->richtung == "right")
	{
		this->pos.at(0).x = this->pos.at(0).x + 10;
	}
	//Hoch
	if (this->richtung == "up")
	{
		this->pos.at(0).y = this->pos.at(0).y - 10;
	}
	//Runter
	if (this->richtung == "down")
	{
		this->pos.at(0).y = this->pos.at(0).y + 10;
	}

}


void Schlange::verlaengern()
{
	//Verhindern dass die Schlange während der Methode gezeichnet wird
	this->draw_freigabe = false;

	//Position des alten letzten Schlangen-Elements nochmal ranhängen, um neues Stück zu erzeugen
	this->pos.push_back(this->pos.at(this->pos.size() - 1));

	//Neu angehängtes Stück verschieben in X (damit die letzten Stücke nicht übereinander liegen)
	if (this->pos.at(this->pos.size() - 1).x > this->pos.at(this->pos.size() - 2).x)
	{
		this->pos.at(this->pos.size() - 1).x = this->pos.at(this->pos.size() - 1).x + 10;
	}
	else
	{
		this->pos.at(this->pos.size() - 1).x = this->pos.at(this->pos.size() - 1).x + 10;
	}

	//Neu angehängtes Stück verschieben in Y (damit die letzten Stücke nicht übereinander liegen)
	if (this->pos.at(this->pos.size() - 1).y > this->pos.at(this->pos.size() - 2).y)
	{
		this->pos.at(this->pos.size() - 1).y = this->pos.at(this->pos.size() - 1).y + 10;
	}
	else
	{
		this->pos.at(this->pos.size() - 1).y = this->pos.at(this->pos.size() - 1).y + 10;
	}

	//Zeichnen der Schlange wieder freigeben
	this->draw_freigabe = true;
}


//+++++++++++++++++++++++++++++++++++++++++++++++++Für Essen++++++++++++++++

int Essen::random_x()
{
	return rand() % 81;
}

int Essen::random_y()
{
	return rand() % 61;
}

void Essen::random_posess()
{
	this->posess.x = (10 * random_x());
	this->posess.y = (10 * random_y());
}