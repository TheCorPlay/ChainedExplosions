#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include <time.h>
#include <cstdlib>

#include "Game.h"

int main(){
	srand(time(NULL));			// Genero una semilla aleatoria
	Game* game = new Game();
	
	game->run();				// Arranco el juego

	delete game;

	return 0;
}