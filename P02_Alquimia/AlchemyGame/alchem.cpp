#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

enum class status
{
_Clave1,
_Clave2,
_Value
};


void main()
{
	std::string key;
	std::string key2;
	std::string content;

	status componente = status::_Value;

	std::string linea;
	std::ifstream elements;
	elements.open("elements.dat");

	std::vector <std::string> formulas;
	std::unordered_map <std::pair<std::string, std::string>, std::string> MapaFormulas;

	while (getline(elements, linea))
	{
		formulas.push_back(linea);
	}
	elements.close();

	
	for (std::vector<std::string>::iterator i = formulas.begin(); i != formulas.end(); ++i) {
		//compruebo que se ha leido el fichero. Y si, lo ha hecho.
		std::cout << *i << std::endl;
	}


	for (std::string e : formulas) //recorrerá el vector formulas y en cada iteracion e será la string de esa posicion
	{
		for (char i : e) //recorrera la string e y en cada iteracion i sera una letra de esta
		{
			switch (i) { //mientras recorro la string, los chars pueden valer '=', '+' o una letra cualquiera.
			case ('='):
				componente = status::_Clave1; //si he leído un igual, se que estoy empezando a leer la key1, así que actualizo el 
												//status a _Clave1. Como lo primero que hay en la línea siempre es el value, el 
												//valor de status es _Value por defecto.
				break;
			case('+'):
				componente = status::_Clave2; //si he leído el '+', lo mismo que antes pero a _Clave2
				break;
			default:
				switch (componente) {  //dependiendo del valor de status guardo los chars en un container distinto
				case(status::_Value):
					content.push_back(i);
					break;
				case(status::_Clave1):
					key.push_back(i);
					break;
				case(status::_Clave2):
					key2.push_back(i);
					break;
				}
				break;
			}
		} //una vez se acaba el foreach y he leído todos los chars de la línea, actualizo el status para que en la siguiente
		//iteración vuelva a guardar los chars en el container "content" hasta que se encuentr el '='
		componente = status::_Value;
		//Meto el pair con el pair de strings (keys) y el string (value)
		MapaFormulas.insert(std::pair <std::pair <std::string, std::string>, std::string>(std::pair <std::string, std::string>(key, key2), content));
		//y reseteo los contenedores donde se guardan las keys y el value.
		content.erase();
		key.erase();
		key2.erase();
	}

	//imprimo para comprobar y va gut
	for (auto i : MapaFormulas) {
		std::cout << i.first.first << "+" << i.first.second << "=" << i.second << std::endl;
	}

	std::vector <std::string> elements_basics{ "Fire", "Water", "Air", "Earth" };

	std::cout << elements_basics.at(0) << std::endl;
	
	//es
}

void IntroduccionComandos() //esta a medias, la ire haciendo a medida que tengamos las funciones
{
	int a;
	int b;

	std::cin >> comandoJugador;

	if (comandoJugador == "add")
	{
		std::cin >> a;

		add(a);
	}
	if (comandoJugador == "add basics " || comandoJugador == "addbasics")
	{
		addBasics();
	}
}

void add(int numero) //duplica un elemento de la lista
{
	currentList.push_back(currentList[numero]);
}

void addBasics()
{
	for (int i = 0; i < elements_basics.size(); i++)
	{
		std::cout << elements_basics[i];
	}

}
