#include <iostream>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include "Input.h"

bool stop=false;//booleano global que permite cerrar la partida si pulsamos esc

//enum class de la dificultad. Le damos a cada dificultad un valor integer para así, más adelante, poder usarla
//para calcular el tamaño del mapa, la cantidad de monedas a recoger y la cantidad de monedas que aparece
enum class dificultad { _EASY = 1, _MEDIUM = 2, _HARDCORE = 3, _MAX = 4 };


//clase mapa
class mapa
{
private:
	//miembros privados fil y col para almacenar la cantidad de filas y columnas que tiene el mapa creado aleatoriamente
	int fil;
	int col;
	//doble puntero de tipo char que nos permite usar un array 2D dinámica
	char **mapusa;

public:

	mapa(dificultad w)
	{
		//mediante el uso de static_cast convertimos la dificultad en integer y calculamos una cantidad aleatoria 
		//de filas y columnas.
		fil = (5 * static_cast<int>(w)) + rand() % ((5 * 2 * static_cast<int>(w)) - (5 * static_cast<int>(w)));
		col = (5 * static_cast<int>(w)) + rand() % ((5 * 2 * static_cast<int>(w)) - (5 * static_cast<int>(w)));
		

		//aquí creamos la array dinámica 2D
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
		for (int i = 0; i < col; i++) {
			std::cout << std::endl;
			for (int j = 0; j < fil; j++) {
					std::cout << mapusa[j][i];
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
	//mimebros para mantener seguimiento de la cantidad de monedas en el mapa y poder acceder, desde dentro
	//de la clase coinmanager a los atributos de mapusa.
	int cantidadMonedas;
	mapa &mapusa;

public:

	CoinManager(mapa &map) :mapusa(map)
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
	//forward declaration ya que usamos atributos de player que aun no han sido definidos

};

class player
{

private:
	int x;
	int y;
	char icono = '@';
	char tecla;
	mapa &mapusa; //al igual que con el coinmanager, cargamos el mapa en referencia, y en este caso
	CoinManager &controlaso; // también cargamos el coinmanager

public:

	int score;  //hemos definido score como public ya que lo hemos considerado como un dato de dominio global
	// dentro de lo que es una partida.

	player(mapa &map, CoinManager &control):mapusa(map),controlaso(control) {
		//el constructor recorre el mapa situandose en posiciones aleatorias hasta encontrar una en la que no hay 
		//monedas. Cuando lo hace, coloca allí al player y deja de ejecutar el doble bucle. También inicializa score en 0.
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
			case Input::Key::W: setY(y - 1);
				break;
			case Input::Key::A: setX(x - 1);
				break;
			case Input::Key::S: setY(y + 1);
				break;
			case Input::Key::D: setX(x + 1);
				break;
			case Input::Key::ENTER:
				break;
			case Input::Key::ESC: stop = true;
				break;
			case Input::Key::NONE:
				break;
		}
	}

	//Usamos setters para que el jugador cambie su posición para así asegurarnos de que si este intenta acceder
	//a una posición fuera del mapa, poder impedirselo

	void setY(int a) {
		if (a >= 0 && a < mapusa.getCol()){
			mapusa.CambiarCelda(x, y, '.');
			y = a;
			controlaso.setMonedas(x, y, score);
			mapusa.CambiarCelda(x, y, icono);
			}
	}

	void setX(int a) {
		if (a >= 0 && a < mapusa.getFil()) {
			mapusa.CambiarCelda(x, y, '.');
			x = a;
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

	Input::Key tecla;

		do {
			tecla = Input::getKey();
			PJ->Movimiento(tecla);
			
			if (gestor->getMonedas() == 0)
				gestor->nuevoConjuntoMonedas();
			map->printMap();
			std::cout << "\n PUNTUACION: " << PJ->score << "/" << coinstoWin;
			system("cls");
		} while (PJ->score!=coinstoWin && !stop); //la partida se ejecutará hasta que el jugador obtenga
		//todas las monedas o pulse el botón ESC

		//si acaba la partida pulsando ESC, no se mostrará por pantalla la felicitación ni el tiempo que ha tardado
		//en acabar
		if (!stop) { 
			std::cout << "Enhorabuena!!! Has recolectado todas las monedas en: " << clock() << " milisegundos." << std::endl;
			int z;
			std::cin >> z;
		}
}

void main() {
	srand(time(nullptr)); //seed aleatoria
	PLAY();
}

