#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <time.h>

using namespace std;

bool spielen = false;

double mx;
double my;

string score_txt = "Score: 0";
string highscore_txt = "High-Score";
int score = 0;
int highscore;
int Sonderobjekte_Zahl = 480;
time_t zeitaktuell; 
time_t zeit_aktualisieren();


//Hilfs Struct für die Position der Schlange
struct Position
{
public:
	int x;
	int y;
};

int bewegung_verlangsamen = 0;


//++++++++++++++++++++++++++Klassen

class Schlange
{
public:
	int laenge = 10;
	int geschwindigkeit = 5;

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
			p.x = p.x + 20;
		}
	}
};

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

class Sonderobjekte: public Essen
{
public:

};

class GameWindow : public Gosu::Window
{

	Gosu::Image apfel;
	Gosu::Image nike;
	Gosu::Image blatt;
	Gosu::Sample biss;
	Gosu::Sample husten;
	Gosu::Sample motorrad;
	
	//Schlange erstellen
	Schlange s;

	// Essen erstellen
	Essen e;

	// Sonderobjekte erstellen
	Sonderobjekte n;
	Sonderobjekte bl;
	
	double apfel_scale_height = 1.0;
	double apfel_scale_width = 1.0;
	double nike_scale_height = 1.0;
	double nike_scale_width = 1.0;
	double blatt_scale_height = 1.0;
	double blatt_scale_width = 1.0;

	Gosu::Font text;

public:
	GameWindow()
		: Window(805, 605, true) 
		, apfel("apfel.png"), text(10),
		biss("biss.mp3"),
		husten("husten.mp3"),
		motorrad("Motorrad.mp3"),
		nike("nike.png"),
		blatt("blatt.png")
	{
		set_caption("Snake by Kevin-Marcel Schnell & Nils Hepp");
		apfel_scale_height = 20.0 / apfel.height();
		apfel_scale_width = 20.0 / apfel.width();
		nike_scale_height = 20.0 / nike.height();
		nike_scale_width = 20.0 / nike.width();
		blatt_scale_height = 20.0 / blatt.height();
		blatt_scale_width = 20.0 / blatt.width();
	}


	int Spielbrettbegrenzung_x_max = 790;
	int Spielbrettbegrenzung_x_min = 10;
	int Spielbrettbegrenzung_y_max = 590;
	int Spielbrettbegrenzung_y_min = 110;


	void draw() override
	{
		//Spielen oder Menu?
		if (spielen) 
		{
			//Schlange zeichnen 
			if (s.draw_freigabe)
			{
				for (size_t i = 0; i < s.pos.size(); i++)
				{
					if (i == 0)
					{
						graphics().draw_quad(
							(s.pos.at(i).x - 10), (s.pos.at(i).y + 10), Gosu::Color::RED,
							(s.pos.at(i).x + 10), (s.pos.at(i).y + 10), Gosu::Color::RED,
							(s.pos.at(i).x - 10), (s.pos.at(i).y - 10), Gosu::Color::RED,
							(s.pos.at(i).x + 10), (s.pos.at(i).y - 10), Gosu::Color::RED,
							0.0);
					}
					else
					{
						graphics().draw_quad(
							(s.pos.at(i).x - 10), (s.pos.at(i).y + 10), Gosu::Color::GRAY,
							(s.pos.at(i).x + 10), (s.pos.at(i).y + 10), Gosu::Color::GRAY,
							(s.pos.at(i).x - 10), (s.pos.at(i).y - 10), Gosu::Color::GRAY,
							(s.pos.at(i).x + 10), (s.pos.at(i).y - 10), Gosu::Color::GRAY,
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

			nike.draw_rot(n.posess.x, n.posess.y, 0.0,
				0.0, // Rotationswinkel in Grad
				0.5, 0.5, // Position der "Mitte" relativ zu x, y
				nike_scale_width, nike_scale_height
			);

			blatt.draw_rot(bl.posess.x, bl.posess.y, 0.0,
				0.0, // Rotationswinkel in Grad
				0.5, 0.5, // Position der "Mitte" relativ zu x, y
				blatt_scale_width, blatt_scale_height
			);

			text.draw_text(score_txt, 100, 20, 0, 5, 5);
			text.draw_text(highscore_txt, 400, 20, 0, 5, 5);
		}
		else 
		{
			graphics().draw_quad(
				(800 / 3), (600 / 3), Gosu::Color::GREEN,
				((800 / 3) * 2), (600 / 3), Gosu::Color::GREEN,
				((800 / 3) * 2), ((600 / 3) * 2), Gosu::Color::GREEN,
				(800 / 3), ((600 / 3) * 2), Gosu::Color::GREEN, 0.0);

			graphics().draw_quad(
				(800 / 3), ((600 / 3) + 200), Gosu::Color::RED,
				((800 / 3) * 2), ((600 / 3) + 200), Gosu::Color::RED,
				((800 / 3) * 2), (((600 / 3) * 2) + 200), Gosu::Color::RED,
				(800 / 3), (((600 / 3) * 2) + 200), Gosu::Color::RED, 0.0);

			text.draw_text("Play", 310, 270, 0, 10, 10);
			text.draw_text("Close", 290, 470, 0, 10, 10);
			text.draw_text(highscore_txt, 250, 120, 0, 5, 5);
			text.draw_text("Snake", 250, 20, 0, 10, 10);
		}
	}


	void update() override
	{
		if (spielen)
		{
			if (s.draw_freigabe)
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

				// Neues Essen erscheinen lassen + Schlange länger machen ++++++++++++++++++++
				if ((s.pos.at(0).x == e.posess.x) && (s.pos.at(0).y == e.posess.y)) {
					biss.play(1, 0.8);
					e.random_posess();
					s.verlaengern();

					//Score aktuallisieren und in String speichern
					score++;
					score_txt = to_string(score);
					score_txt = "Score: " + score_txt;

					if (score % 7 == 0) { //Nikes Spawnen lassen wenn score bei ca 7
						n.random_posess();
					}

					if (score % 11 == 0){
						bl.random_posess();
					}

					//ggfs. Highscore aktualisieren und in Textdatei schreiben
					if (score > highscore)
					{
						highscore = score;
						ofstream x("HighScore.txt");
						x << highscore << endl;
						highscore_txt = to_string(highscore);
						highscore_txt = "High-Score: " + highscore_txt;
					}
				}

				// Nikes-Essen
				if ((s.pos.at(0).x == n.posess.x) && (s.pos.at(0).y == n.posess.y)) {
					motorrad.play(1, 1);
					n.posess.x = 1000;
					n.posess.y = 1000;
					s.verlaengern();

					//Score aktuallisieren und in String speichern
					score = score + 2;
					score_txt = to_string(score);
					score_txt = "Score: " + score_txt;

					//ggfs. Highscore aktualisieren und in Textdatei schreiben
					if (score > highscore)
					{
						highscore = score;
						ofstream x("HighScore.txt");
						x << highscore << endl;
						highscore_txt = to_string(highscore);
						highscore_txt = "High-Score: " + highscore_txt;
					}

					// Für 8 sek schneller machen
					s.geschwindigkeit = 3;
					Sonderobjekte_Zahl = 0;
				}

				// Blatt-Essen
				if ((s.pos.at(0).x == bl.posess.x) && (s.pos.at(0).y == bl.posess.y)) {
					husten.play(1, 1);
					bl.posess.x = 1100;
					bl.posess.y = 1100;
					s.geschwindigkeit = 2;
					s.verlaengern();

					//Geschwindigkeit senken
					s.geschwindigkeit = 8;
					Sonderobjekte_Zahl = 0;

					//Score aktuallisieren und in String speichern
					score++;
					score_txt = to_string(score);
					score_txt = "Score: " + score_txt;

					//ggfs. Highscore aktualisieren und in Textdatei schreiben
					if (score > highscore)
					{
						highscore = score;
						ofstream x("HighScore.txt");
						x << highscore << endl;
						highscore_txt = to_string(highscore);
						highscore_txt = "High-Score: " + highscore_txt;
					}

				}
				if (Sonderobjekte_Zahl <= 480) {
					Sonderobjekte_Zahl++;
				}
				if (Sonderobjekte_Zahl == 480) {
					s.geschwindigkeit = 5;
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
					spielen = false; 
				}

				// Falls Schlange sich selbst essen will
				for (size_t i = 1; i < s.pos.size(); i++)
				{
					if ((s.pos.at(0).x == s.pos.at(i).x) && (s.pos.at(0).y == s.pos.at(i).y))
					{
						spielen = false;
					}
				}
			}

		}
		
		else 
		{
			mx = input().mouse_x();
			my = input().mouse_y();
			//Play
			if (input().down(Gosu::MS_LEFT) && (mx > (800 / 3)) && (mx < ((800 * 2) / 3)) && (my > (600 / 3)) && (my < (600 * 2 / 3)))
			{
				s.pos.clear();
				Position p = { 400,300 };
				s.geschwindigkeit = 5;
				score = 0;
				score_txt = to_string(score);
				score_txt = "Score: " + score_txt;
				s.richtung = "left";
				for (int i = 0; i < 10; i++)
				{
					this->s.pos.push_back(p);
					p.x = p.x + 20;
				}
				spielen = true;
			}
			//Close
			if (input().down(Gosu::MS_LEFT) && (mx > (800 / 3)) && (mx < ((800 * 2) / 3)) && (my > ((600 / 3)) + 200) && (my < ((600 * 2 / 3) + 200)))
			{
				Window::close();
			}
		}
	}
};

// C++ Hauptprogramm
int main()
{
	srand(time(NULL));
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

//++++++++++++++++++++++Methoden

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
		this->pos.at(0).x = this->pos.at(0).x - 20;
	}
	//Rechts
	if (this->richtung == "right")
	{
		this->pos.at(0).x = this->pos.at(0).x + 20;
	}
	//Hoch
	if (this->richtung == "up")
	{
		this->pos.at(0).y = this->pos.at(0).y - 20;
	}
	//Runter
	if (this->richtung == "down")
	{
		this->pos.at(0).y = this->pos.at(0).y + 20;
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
		this->pos.at(this->pos.size() - 1).x = this->pos.at(this->pos.size() - 1).x + 20;
	}
	else
	{
		this->pos.at(this->pos.size() - 1).x = this->pos.at(this->pos.size() - 1).x + 20;
	}

	//Neu angehängtes Stück verschieben in Y (damit die letzten Stücke nicht übereinander liegen)
	if (this->pos.at(this->pos.size() - 1).y > this->pos.at(this->pos.size() - 2).y)
	{
		this->pos.at(this->pos.size() - 1).y = this->pos.at(this->pos.size() - 1).y + 20;
	}
	else
	{
		this->pos.at(this->pos.size() - 1).y = this->pos.at(this->pos.size() - 1).y + 20;
	}

	//Zeichnen der Schlange wieder freigeben
	this->draw_freigabe = true;
}


int Essen::random_x()
{
	return rand() % 39;
}

int Essen::random_y()
{
	return rand() % 23;
}

void Essen::random_posess()
{
	this->posess.x = (20 * random_x() + 20);
	this->posess.y = (20 * random_y() + 120);
}
