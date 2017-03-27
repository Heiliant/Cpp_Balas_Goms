#include <iostream>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include "Input.h"

enum class dificultad { _EASY = 1, _MEDIUM = 2, _HARDCORE = 3, _SATAN = 4, _MAX = 5 };

class player; //la defino aqui porque sino en coin manager no la detectara ya que esta hecha antes

class mapa
{
private:

	int fil;
	int col;

	char **mapusa;

public:

	mapa(dificultad w)
	{
		fil = (5 * static_cast<int>(w)) + rand() % ((5 * 2 * static_cast<int>(w)) - (5 * static_cast<int>(w)));
		col = (5 * static_cast<int>(w)) + rand() % ((5 * 2 * static_cast<int>(w)) - (5 * static_cast<int>(w)));
		
		mapusa = new char *[fil];

		for (int i = 0; i < fil; i++)
		{
			mapusa[i] = new char[col];
		}


		for (int i = 0; i < fil; i++)
		{
			for (int j = 0; j < col; j++)
			{
				mapusa[i][j] = '.';
			}
		}

	}

	void CambiarCelda(int filas, int columnas, char contenido)
	{
		mapusa[filas][columnas] = contenido;
	}

	void printMap() {
		for (int i = 0; i < fil; i++) {
			for (int j = 0; j < col; j++) {
				if (j != col - 1)
					std::cout << mapusa[i][j];
				else
					std::cout << mapusa[i][j] << std::endl;
			}
		}
	}

	int getFil() {
		return fil;
	}
	int getCol() {
		return col;
	}
	char** getMapusa() {
		return mapusa;
	}
};

class CoinManager
{
private:

	int cantidadMonedas;
	mapa &mapusa;

public:

	CoinManager(mapa &map) :mapusa(map) //pasamos el mapa por referencia y guardamos esa referencai en mapusa, 
										//asi si necesitamos tocar el mapa en otra funcion usamos mapusa
	{
		cantidadMonedas = static_cast<float>(map.getFil())*static_cast<float>(map.getCol())*((static_cast<float>(rand() % 10 + 3)) / 100);

		for (int i = 0; i < cantidadMonedas;) {
			int j = rand() % mapusa.getFil();
			int k = rand() % mapusa.getCol();

			if (mapusa.getMapusa()[j][k] == '.') {
				mapusa.CambiarCelda(j, k, '$');
				i++;
			}
		}
	}

	void nuevoConjuntoMonedas() {
		cantidadMonedas = static_cast<float>(mapusa.getFil())*static_cast<float>(mapusa.getCol())*
							((static_cast<float>(rand() % 10 + 3)) / 100);

		for (int i = 0; i < cantidadMonedas;) {
			int j = rand() % mapusa.getFil();
			int k = rand() % mapusa.getCol();

			if (mapusa.getMapusa()[j][k] == '.') {
				mapusa.getMapusa()[j][k] = '$';
				i++;
			}
		}
	}
	
	int getMonedas() {
		return cantidadMonedas;
	}

	void setMonedas(int x, int y, int &punt);

};

class player
{

private:
	int x;
	int y;
	char icono = '@';
	char tecla;
	mapa &mapusa;
	CoinManager &controlaso;

public:

	int score;

	player(mapa &map, CoinManager &control):mapusa(map),controlaso(control) {
		score = 0;
		bool located = false;
		for (int i=rand()%map.getFil(); i < map.getFil() && !located; i++) {

			for (int j=rand()%map.getCol(); j < map.getCol() && !located; j++){

				if (map.getMapusa()[i][j] != '$'){
					map.CambiarCelda(i, j, icono);
					x = i;
					y = j;
					located=true;
				}
			}
		}
	}

	void Movimiento(Input::Key tecla)
	{
		switch (tecla) {
			case Input::Key::W: setX(x - 1);
				break;
			case Input::Key::A: setY(y - 1);
				break;
			case Input::Key::S: setX(x + 1);
				break;
			case Input::Key::D: setY(y + 1);
				break;
			case Input::Key::ENTER:
				break;
			case Input::Key::ESC:
				break;
			case Input::Key::NONE:
				break;
		}
	}

	void setX(int a) {
		if (a >= 0 && a < mapusa.getCol()){
			mapusa.CambiarCelda(x, y, '.');
			x = a;
			controlaso.setMonedas(x, y, score);
			mapusa.CambiarCelda(x, y, icono);
			}
	}

	void setY(int a) {
		if (a >= 0 && a < mapusa.getFil()) {
			mapusa.CambiarCelda(x, y, '.');
			y = a;
			controlaso.setMonedas(x, y, score);
			mapusa.CambiarCelda(x, y, icono);
		}
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}
};

void CoinManager::setMonedas(int x, int y, int &punt) {
	if (mapusa.getMapusa()[x][y] == '$') {
		punt++;
		cantidadMonedas--;
	}
}

void PLAY() {
	dificultad a;
	char b;
	std::cout << "\n\t\t ***COIN RACE***\n\n" << std::endl;
	repeat:
	std::cout << "\t Por favor, seleccione un nivel de dificultad:\n" << std::endl;
	std::cout << "\t\t 1. FACIL \n\t\t 2. INTERMEDIO \n\t\t 3. DIFICIL \n" << std::endl;
	std::cin >> b;
	switch (b) {
	case 49: a = dificultad::_EASY;
		break;
	case 50: a = dificultad::_MEDIUM;
		break;
	case 51: a = dificultad::_HARDCORE;
		break;
	default: std::cout << "Caracter de entrada no valido (pulsa 1, 2 o 3 para seleccionar la dificultad\n" << std::endl; goto repeat;
		break;
	}

	mapa* map=new mapa(a);
	CoinManager* gestor = new CoinManager(*map);
	player *PJ=new player(*map, *gestor);


	int coinstoWin = (static_cast<int>(a) * 30) + rand() % ((static_cast<int>(a) * 30 * 2) - (static_cast<int>(a) * 30));
	std::cout << "Deberas coleccionar u total de " << coinstoWin << " monedas para ganar. BUENA SUERTE!" << std::endl;
	std::cout << "pulsa ENTER para empezar a jugar" << std::endl;

	Input::Key tecla;

		do {
			tecla = Input::getKey();
			PJ->Movimiento(tecla);
			
			if (gestor->getMonedas() == 0)
				gestor->nuevoConjuntoMonedas();
			map->printMap();
			std::cout << "\n PUNTUACION: " << PJ->score << "/" << coinstoWin;
			system("cls");
		} while (PJ->score!=coinstoWin);
		std::cout << "Enhorabuena!!! Has recolectado todas las monedas en: " << clock() << " segundos." << std::endl;
		std::cin;
}

void main() {
	srand(time(nullptr));
	PLAY();
}

