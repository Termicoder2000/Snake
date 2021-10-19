#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
//Wozu time ?
#include <time.h>

using namespace std;

string score_txt = "Score: 0";
string highscore_txt = "High-Score";
int score = 0;
int highscore;


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
	


public:
	
	Position posess;

	// random
	void random_posess();
	int random_x();
	int random_y();

	//Konstruktor des Essens
	Essen()
	{
		//Startwert festlegen
		random_posess();

	}
};
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Sonderobjekte
class Sonderobjekte
{
	Position posess;

	// random
	void random_posess();
	int random_x();
	int random_y();

	//Konstruktor des Essens
	Essen()
	{
		//Startwert festlegen
		random_posess();

	}
};

class GameWindow : public Gosu::Window
{

	Gosu::Image apfel;
	Gosu::Image Nike;
	Gosu::Sample biss;
	
	//Schlange erstellen
	Schlange s;

	// Essen erstellen
	Essen e;
	double apfel_scale_height = 1.0;
	double apfel_scale_width = 1.0;

	Gosu::Font text;

public:
	GameWindow()
		: Window(805, 605)//, true) noch anhängen, wenn Vollbild benötigt wird.
		, apfel("apfel.png")
	{
		set_caption("Snake by Kevin-Marcel Schnell & Nils Hepp");
		apfel_scale_height = 10.0 / apfel.height();
		apfel_scale_width = 10.0 / apfel.width();

	}


	int Spielbrettbegrenzung_x_max = 795;
	int Spielbrettbegrenzung_x_min = 5;
	int Spielbrettbegrenzung_y_max = 595;
	int Spielbrettbegrenzung_y_min = 105;


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

		graphics().draw_line(Spielbrettbegrenzung_x_max, Spielbrettbegrenzung_y_max, Gosu::Color::WHITE, Spielbrettbegrenzung_x_max, Spielbrettbegrenzung_y_min, Gosu::Color::WHITE, 0.0);
		graphics().draw_line(Spielbrettbegrenzung_x_max, Spielbrettbegrenzung_y_max, Gosu::Color::WHITE, Spielbrettbegrenzung_x_min, Spielbrettbegrenzung_y_max, Gosu::Color::WHITE, 0.0);
		graphics().draw_line(Spielbrettbegrenzung_x_min, Spielbrettbegrenzung_y_min, Gosu::Color::WHITE, Spielbrettbegrenzung_x_max, Spielbrettbegrenzung_y_min, Gosu::Color::WHITE, 0.0);
		graphics().draw_line(Spielbrettbegrenzung_x_min, Spielbrettbegrenzung_y_min, Gosu::Color::WHITE, Spielbrettbegrenzung_x_min, Spielbrettbegrenzung_y_max, Gosu::Color::WHITE, 0.0);
		// Essen zeichnen ++++++++++++++++++++++++++++++++++++++++++++++
		apfel.draw_rot(e.posess.x, e.posess.y, 0.0,
			0.0, // Rotationswinkel in Grad
			0.5, 0.5, // Position der "Mitte" relativ zu x, y
			apfel_scale_width, apfel_scale_height
		);

		/*
		graphics().draw_quad(
			(e.posess.x - 5), (e.posess.y + 5), Gosu::Color::GREEN,
			(e.posess.x + 5), (e.posess.y + 5), Gosu::Color::GREEN,
			(e.posess.x - 5), (e.posess.y - 5), Gosu::Color::GREEN,
			(e.posess.x + 5), (e.posess.y - 5), Gosu::Color::GREEN,
			0.0);
		
		/*graphics().draw_rect(
			e.posess.x, e.posess.y, 10, 10, Gosu::Color::GREEN, 0
		);*/


		//https://www.libgosu.org/cpp/class_gosu_1_1_font.html#a86067397eacecc5fd88f447038a88b1d Damit Score und High-Score auf Bildschirm ausgeben
		//Int muss noch zu String umgewandelt werden!!

		void Gosu::Font::draw_text(score_txt,
			10,
			10,
			0.0,
			1,
			1,
			Color::WHITE,
			AM_DEFAULT
		)		const
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
			biss.play(1, 0.8) ;
			s.geschwindigkeit--;
			e.random_posess();
			s.verlaengern();
			
			//Score aktuallisieren und in String speichern
			score++;
			score_txt = to_string(score);
			score_txt = "Score: " + score_txt;

			//ggfs. Highscore aktuallisieren und in Tetdatei schreiben
			if (score > highscore)
			{
				highscore = score;
				ofstream x("HighScore.txt");
				x << highscore << endl;
				highscore_txt = to_string(highscore);
				highscore_txt = "High-Score: " + highscore_txt;
			}
		}

		//Geschwindigkeitsregelung für die Schlange (nicht Linear!!)
		if (bewegung_verlangsamen > s.geschwindigkeit)
		{
			s.move();
			bewegung_verlangsamen = 0;
		}
		bewegung_verlangsamen++;

		
		// Spielfeldbegrenzung ++++++++++++++++++++++++++++++++
		if (s.pos.at(0).x <= Spielbrettbegrenzung_x_min || s.pos.at(0).x > Spielbrettbegrenzung_x_max || s.pos.at(0).y <= Spielbrettbegrenzung_y_min || s.pos.at(0).y > Spielbrettbegrenzung_y_max) {
		Window::close(); // Schließt nur, lässt nicht neu starten.
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
	//High-Score Einlesen
	ifstream f("HighScore.txt");
	if (f >> highscore)
	{
		highscore_txt = to_string(highscore);
		highscore_txt = "High-Score: " + highscore_txt;
		cout << highscore_txt << endl;
	}
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