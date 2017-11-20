#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Item.h"
#include "LvlUp.h"

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <vector>
#include <utility>
#include <map>

class Game {
private:
	//Constantes
	const static int DIM = 12;

	// Variables de dibujo
	sf::RenderWindow ventana;
	sf::Event eventosVentana;

	sf::Texture texturaFondo, tItems, tExplosion, tLvlUp;
	sf::Sprite graficoFondo, gItems, gExplosion, gLvlUp;
	sf::Font fTitulo;

	int timeExp;	// Tiempo de la explosión (va desapareciendo)
	int timeGeneradas;

	// Variables de sonido
	sf::Music msFondo;

	sf::SoundBuffer buffSonidos;
	sf::Sound sndExplosion;

	bool silenced;

	/// Variables de eventos
	bool leftClick;
	int mouseX, mouseY;

	// Variables de la lógica
	ItemBasic itemBasic;

	bool isPlay;

	int tablero[DIM][DIM];					// tablero[fila][columna]
	bool tableroPasadas[DIM][DIM];			// tablero de actualizadas
	bool tableroPintarExplosion[DIM][DIM];	// tablero con las explosiones a pintar
	bool tableroRecienGeneradas[DIM][DIM];	// tablero de generado en este turno
	// Contiene un map con los lvls up en el tablero
	// Coordenadas a el lvlUp concreto
	std::map <std::pair <int, int> , LvlUp> mapLvlsUp;

	bool generaLvlUP;
	bool reseteado;							// Me dirá si el tableroPasadas lo he reseteado
	bool resetRecienGeneradas;				// Me dirá si el tableroRecienGeneradas lo he reseteado

	int score;
	int lvl;
	int actualP;
	int progresion;

	// Detección de teclas
	bool addPressed;
	bool subPressed;

private:
	void inicializar();
	void reset();							// Básicamente es un reset, pero ahorrandome una serie de cosas
	void eventos();
	void update();
	void render();
	void cleared();

	// Metodos de la lógica

	void resetearTableroPasadas();
	void resetearTableroGeneradas();
	std::vector<std::pair<int, int> > libres();
	bool generar();
	bool colocar(const std::pair<int,int> &pos);
	void chained(const std::pair<int, int> &pos, ItemBasic &miBomba);
	void ejecutarExplosion(const std::vector<std::pair <int, int> > &vectorExp);
	void explosionAmarilla(const std::pair<int, int> &pos);
	void explosionNaranja(const std::pair<int, int> &pos);
	void explosionRoja(const std::pair<int, int> &pos);
	void endGame();

	// Metodos de graficos

public:
	// Datos
	std::string version = "v2.0.0";

	Game();
	void run();
};

#endif // GAME_H_INCLUDED