#include "Game.h"
#include "Item.h"
#include "LvlUp.h"

#include <exception>
#include <vector>
#include <utility>
#include <cstdlib>
#include <string>

#include <iostream>

/*
	Constructor de la clase Game
*/
Game::Game() {
	// Variable del bucle
	isPlay = true;

	/// Variables de eventos
	leftClick = false;
	mouseX = 0;
	mouseY = 0;

	ventana.create(sf::VideoMode(554, 600), "Chained Explosions - " + version, sf::Style::Titlebar | sf::Style::Close);	// Creo la ventana
}

void Game::inicializar() {
	ventana.setFramerateLimit(60);																			// Limite de FPS
	ventana.setVerticalSyncEnabled(true);																	// Se verá mejor los movimientos
	ventana.setKeyRepeatEnabled(false);																		// Me permitirá detectar mejor cada letra

	if (!texturaFondo.loadFromFile("images/fondo.png") ||
		!tItems.loadFromFile("images/items.png") ||
		!tExplosion.loadFromFile("images/explosion.png") ||
		!tLvlUp.loadFromFile("images/lvlUp.png") ||
		!fTitulo.loadFromFile("fonts/Sheeping Dogs.ttf")) {
		// Ya lanzará un error de carga de imagenes
		throw std::exception("Error en la carga de gráficos.");
	}

	if (!buffSonidos.loadFromFile("sounds/Explosion.wav") ||
		!msFondo.openFromFile("sounds/fatRat.ogg")) {
		// Ya lanzará un error de carga de sonidos
		throw std::exception("Error en la carga de sonidos.");
	}

	// Referente a graficos
	gItems.setTexture(tItems);
	gExplosion.setTexture(tExplosion);
	gLvlUp.setTexture(tLvlUp);
	graficoFondo.setTexture(texturaFondo);
	graficoFondo.setPosition(sf::Vector2f(0.f,0.f));

	timeExp = 0;
	timeGeneradas = 0;

	// Referente a sonidos

	silenced = false;
	msFondo.setLoop(true);
	msFondo.setVolume(50);
	msFondo.play();

	sndExplosion.setBuffer(buffSonidos);
	sndExplosion.setVolume(100);

	// Preparo todo lo referente a la lógica
	lvl = 1;
	score = 0;
	progresion = 1;
	actualP = 0;
	reseteado = true;
	generaLvlUP = false;

	addPressed = false;
	subPressed = false;

	for (int i = 0; i < DIM; i++) {	// Aquí reseteo manualmente para rellenar de paso el tablero de 0's
		for (int j = 0; j < DIM; j++) {
			tablero[i][j] = 0;
			tableroPasadas[i][j] = false;
			tableroRecienGeneradas[i][j] = false;
			tableroPintarExplosion[i][j] = false;
		}
	}

	// Preparo cómo serán las explosiones
	itemBasic = ItemBasic();

	generar();
}

void Game::reset() {
	// Referente a gráficos
	timeExp = 0;
	timeGeneradas = 0;

	// Referente a sonidos
	//silenced = false;

	// Preparo todo lo referente a la lógica
	lvl = 1;
	score = 0;
	progresion = 1;
	actualP = 0;
	reseteado = true;
	generaLvlUP = false;

	for (int i = 0; i < DIM; i++) {	// Aquí reseteo manualmente para rellenar de paso el tablero de 0's
		for (int j = 0; j < DIM; j++) {
			tablero[i][j] = 0;
			tableroPasadas[i][j] = false;
			tableroRecienGeneradas[i][j] = false;
			tableroPintarExplosion[i][j] = false;
		}
	}

	// Preparo cómo serán las explosiones
	//itemBasic = ItemBasic();
	itemBasic.resetLvl();

	generar();
}

void Game::eventos() {
	while (ventana.pollEvent(eventosVentana)) {
		if (eventosVentana.type == sf::Event::Closed) { // Si pulsa cerrar
			cleared();
		}
		if (eventosVentana.type == sf::Event::KeyPressed && eventosVentana.key.code == sf::Keyboard::M) {	// Si ha pulsado la M (mute)
			if (silenced) {
				msFondo.play();
				silenced = false;
			}
			else {
				msFondo.pause();
				silenced = true;
			}
		}
		if (eventosVentana.type == sf::Event::KeyPressed && eventosVentana.key.code == sf::Keyboard::Add) {
			addPressed = true;
		}
		if (eventosVentana.type == sf::Event::KeyPressed && eventosVentana.key.code == sf::Keyboard::Subtract) {
			subPressed = true;
		}
		if (eventosVentana.type == sf::Event::KeyPressed && eventosVentana.key.code == sf::Keyboard::R) {	// Si ha pulsado la R (reset)
			reset();
		}
		if (eventosVentana.type == sf::Event::KeyReleased && eventosVentana.key.code == sf::Keyboard::Add) {	// Si ha levantado el +
			addPressed = false;
		}
		if (eventosVentana.type == sf::Event::KeyReleased && eventosVentana.key.code == sf::Keyboard::Subtract) {	// Si ha levantado el -
			subPressed = false;
		}
		if (eventosVentana.type == sf::Event::MouseMoved) {													// Si he movido el ratón
			mouseX = eventosVentana.mouseMove.x;
			mouseY = eventosVentana.mouseMove.y;
		}

		// Si hago click izquierdo
		if (eventosVentana.type == sf::Event::MouseButtonPressed && eventosVentana.mouseButton.button == sf::Mouse::Left) {
			std::pair <int, int> posRaton(mouseY, mouseX);									// Guardo la posición del ratón cuando haces click

			if (posRaton.first >= 20 && posRaton.first <= 535) {							// Si estoy en el rango válido
				if (posRaton.second >= 20 && posRaton.second <= 535) {
					posRaton.first -= 20;
					posRaton.second -= 20;
					/// Cada celda es de 43x43 pixeles
					posRaton.first = posRaton.first / 43;
					posRaton.second = posRaton.second / 43;
					/// Ahora tenemos en posRaton directamente el valor de la matriz que queremos colocar
					if (colocar(posRaton) && !silenced)
						sndExplosion.play();
				}
			}
		}
	}
}

/// Lógica del programa (interacción con el usuario)
void Game::update() {
	if (addPressed) {
		if (msFondo.getVolume() < 100.f)
			msFondo.setVolume(msFondo.getVolume() + 1.f);
		if (sndExplosion.getVolume() < 100.f)
			sndExplosion.setVolume(sndExplosion.getVolume() + 1.f);
	}
	if (subPressed) {
		if (msFondo.getVolume() > 1.f)
			msFondo.setVolume(msFondo.getVolume() - 1.f);
		if (sndExplosion.getVolume() > 1.f)
			sndExplosion.setVolume(sndExplosion.getVolume() - 1.f);
	}
}

// Mostrar (se ejecutará 60 veces por segundo)
void Game::render() {
	// Create a text
	sf::Text text ("Lvl: " + std::to_string(lvl), fTitulo);
	sf::Text textScore("Score: " + std::to_string(score), fTitulo);

	text.setCharacterSize(30);
	textScore.setCharacterSize(30);
	text.setPosition(63.f,545.f);
	textScore.setPosition(364.f, 545.f);

	ventana.clear(sf::Color(120,120,120));			// Limpiamos la pantalla
	ventana.draw(graficoFondo);						// Fondo

	if (!reseteado) { // Auxiliar para ir difuminando las explosiones hasta borrarlas (en 0,425 segundos se borra)
		timeExp += 10;
		if (timeExp >= 255) {
			timeExp = 0;
			resetearTableroPasadas();
		}
	}

	if (!resetRecienGeneradas) {
		timeGeneradas += 10;
		if (timeGeneradas >= 255) {
			timeGeneradas = 255;
			resetearTableroGeneradas();
		}
	}

	// Dibujar los lvls up
	for (std::map <std::pair <int, int>, LvlUp>::iterator it = mapLvlsUp.begin(); it != mapLvlsUp.end(); ++it) {
		gLvlUp.setTextureRect(sf::IntRect((*it).second.getGrafico() * 43 + 6 * (*it).second.getGrafico(),
										  (*it).second.getEstadoGrafico() * 43 + 6 * (*it).second.getEstadoGrafico(), 43, 43));
		(*it).second.timeLvl += 1;
		gLvlUp.setColor(sf::Color(255, 255, 255, 255 - (*it).second.timeLvl*10));
		
		if ((*it).second.timeLvl > 17) {
			(*it).second.nextEstadoGrafico();
			(*it).second.timeLvl = 0;
		}
		gLvlUp.setPosition(sf::Vector2f(19.f + 43 * (*it).first.second, 19.f + 43 * (*it).first.first));
		ventana.draw(gLvlUp);
	}

	/// Dibuja las fichas y explosiones del tablero
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (tableroRecienGeneradas[i][j]) {
				gItems.setColor(sf::Color(255, 255, 255, timeGeneradas));
			}
			else {
				gItems.setColor(sf::Color(255, 255, 255, 255));
			}
			if (tablero[i][j] != 0 && tablero [i][j] != 5) { // Fichas
				gItems.setTextureRect(sf::IntRect(43*(tablero[i][j] - 1), 0, 43, 43));
				gItems.setPosition(sf::Vector2f(19.f+43*j, 19.f+43*i));		
				ventana.draw(gItems);
			}
			if (tableroPintarExplosion[i][j]) { // Explosiones
				gExplosion.setTextureRect(sf::IntRect(0, 0, 43, 43));
				gExplosion.setPosition(sf::Vector2f(19.f + 43 * j, 19.f + 43 * i));
				gExplosion.setColor(sf::Color(255,255,255,255 - timeExp));
				ventana.draw(gExplosion);
			}
		}
	}

	ventana.draw(text);
	ventana.draw(textScore);

	ventana.display();
}

// Cerrar todo
void Game::cleared() {
	ventana.close();
	isPlay = false;
}

// Se encargará de hacerlo todo
void Game::run() {
	inicializar();
	
	while (isPlay) {
		eventos();
		update();
		render();
	}

	cleared();
}

void Game::resetearTableroGeneradas() {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			tableroRecienGeneradas[i][j] = false;
		}
	}

	timeGeneradas = 0;
	resetRecienGeneradas = true;
}

void Game::resetearTableroPasadas() {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			tableroPasadas[i][j] = false;
			tableroPintarExplosion[i][j] = false;
		}
	}
	reseteado = true;
}

std::vector< std::pair <int,int> > Game::libres() {
	std::vector< std::pair<int,int> > vector;

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (tablero[i][j] == 0)
				vector.push_back(std::pair<int, int> (i,j)); // Meto el par i, j si es que está vacío
		}
	}

	return vector;
}

bool Game::generar() {
	int generadas = 0;
	std::vector< std::pair<int, int> > libres = Game::libres();

	if (actualP == progresion) {
		actualP = 0;
		progresion++;
		if (progresion > 5 && progresion % 3 == 0) { // Solo genero 1 de cada 3 veces que llegue al limite (se generaban demasiadas upgrades)
			generaLvlUP = true;
		}
	}
	
	actualP++;

	while (generadas < progresion) {
		if (libres.size() == 0)
			return false;

		int pos = (int) rand() % libres.size();			// Selecciono una posición del vector
		std::pair<int, int> par = libres.at(pos);		// Cojo el par

		if (generaLvlUP && (rand() % 3 == 0)) {
			tablero[par.first][par.second] = 5;				// Genero un LvlUp
			mapLvlsUp[par] = LvlUp ();
			generaLvlUP = false;
		} else {
			tablero[par.first][par.second] = 1;				// Genero un obstáculo
		}

		tableroRecienGeneradas[par.first][par.second] = true;
		libres.erase(libres.begin() + pos);				// Borro el elemento que he añadido
		generadas++;
	}
	
	return libres.size() != 0;								// Devolverá true si queda algún hueco para el usuario, false en otro caso
}

bool Game::colocar(const std::pair <int,int> &pos) {
	if (tablero[pos.first][pos.second] == 5) {
		tablero[pos.first][pos.second] = 0;
		itemBasic.aumentaLvl();
		mapLvlsUp.erase(pos);	// Borro el lvl up del mapa
	}

	if (tablero[pos.first][pos.second] != 0) {			// Si está ocupada y no es un LvlUp
		return false;
	}

	lvl++;

	resetearTableroPasadas();
	resetearTableroGeneradas();

	tablero[pos.first][pos.second] = 2;									// Introduzco una bomba básica
	ejecutarExplosion (itemBasic.explosionAmarilla(pos, DIM));			// Desde pos, explota
	chained(pos, itemBasic);

	// Referente a los graficos
	resetRecienGeneradas = false;
	reseteado = false;
	timeExp = 0;
	timeGeneradas = 0;

	// Genero fichas nuevas
	if (!generar()) {
		endGame();
	}

	return true;
}

void Game::ejecutarExplosion(const std::vector<std::pair <int, int> > &vectorExp) {
	for (std::vector<std::pair <int, int> >::const_iterator it = vectorExp.cbegin(); it != vectorExp.cend(); ++it) {
		if (tablero[(*it).first][(*it).second] == 1) {
			tablero[(*it).first][(*it).second] = 0;
			score++;
		}
		if (tablero[(*it).first][(*it).second] == 0)
			tableroPasadas[(*it).first][(*it).second] = true;
		tableroPintarExplosion[(*it).first][(*it).second] = true;
	}
}

// Método más complejo de la lógica del programa, mirará todos los adyacentes y los hará explotar en cadena
void Game::chained(const std::pair<int, int> &pos, ItemBasic &miBomba) {
	tableroPasadas[pos.first][pos.second] = true;				// Hemos pasado por la posición que estoy mirando
	tableroPintarExplosion[pos.first][pos.second] = true;

	// Mirar arriba
	if (pos.first - 1 >= 0 && !tableroPasadas[pos.first - 1][pos.second]) {
		tableroPasadas[pos.first - 1][pos.second] = true;		// Actualizo, está ya ha sido mirada

		switch (tablero[pos.first - 1][pos.second]){
		case 2:
			tablero[pos.first - 1][pos.second] = 3;
			ejecutarExplosion (miBomba.explosionAmarilla(std::pair<int,int>(pos.first-1, pos.second), DIM));
			chained(std::pair<int, int>(pos.first - 1, pos.second), miBomba);
			break;
		case 3:
			tablero[pos.first - 1][pos.second] = 4;
			ejecutarExplosion (miBomba.explosionNaranja(std::pair<int, int>(pos.first - 1, pos.second), DIM));
			chained(std::pair<int, int>(pos.first - 1, pos.second), miBomba);
			break;
		case 4:
			tablero[pos.first - 1][pos.second] = 0;
			ejecutarExplosion (miBomba.explosionRoja(std::pair<int, int>(pos.first - 1, pos.second), DIM));
			chained(std::pair<int, int>(pos.first - 1, pos.second), miBomba);
			break;
		default:
			break;
		}
	}

	// Mirar abajo
	if (pos.first + 1 < DIM && !tableroPasadas[pos.first + 1][pos.second]) {
		tableroPasadas[pos.first + 1][pos.second] = true;		// Actualizo, está ya ha sido mirada
		switch (tablero[pos.first + 1][pos.second]){
		case 2:
			tablero[pos.first + 1][pos.second] = 3;
			ejecutarExplosion (miBomba.explosionAmarilla(std::pair<int, int>(pos.first + 1, pos.second), DIM));
			chained(std::pair<int, int>(pos.first + 1, pos.second), miBomba);
			break;
		case 3:
			tablero[pos.first + 1][pos.second] = 4;
			ejecutarExplosion (miBomba.explosionNaranja(std::pair<int, int>(pos.first + 1, pos.second), DIM));
			chained(std::pair<int, int>(pos.first + 1, pos.second), miBomba);
			break;
		case 4:
			tablero[pos.first + 1][pos.second] = 0;
			ejecutarExplosion (miBomba.explosionRoja(std::pair<int, int>(pos.first + 1, pos.second), DIM));
			chained(std::pair<int, int>(pos.first + 1, pos.second), miBomba);
			break;
		default:
			break;
		}
	}

	// Mirar izquierda
	if (pos.second - 1 >= 0 && !tableroPasadas[pos.first][pos.second - 1]) {
		tableroPasadas[pos.first][pos.second- 1] = true;		// Actualizo, está ya ha sido mirada
		switch (tablero[pos.first][pos.second - 1]){
		case 2:
			tablero[pos.first][pos.second - 1] = 3;
			ejecutarExplosion (miBomba.explosionAmarilla(std::pair<int, int>(pos.first, pos.second - 1), DIM));
			chained(std::pair<int, int>(pos.first, pos.second - 1), miBomba);
			break;
		case 3:
			tablero[pos.first][pos.second - 1] = 4;
			ejecutarExplosion (miBomba.explosionNaranja(std::pair<int, int>(pos.first, pos.second - 1), DIM));
			chained(std::pair<int, int>(pos.first, pos.second - 1), miBomba);
			break;
		case 4:
			tablero[pos.first][pos.second - 1] = 0;
			ejecutarExplosion (miBomba.explosionRoja(std::pair<int, int>(pos.first, pos.second - 1), DIM));
			chained(std::pair<int, int>(pos.first, pos.second - 1), miBomba);
			break;
		default:
			break;
		}
	}

	// Mirar derecha
	if (pos.second + 1 < DIM && !tableroPasadas[pos.first][pos.second + 1]) {
		tableroPasadas[pos.first][pos.second + 1] = true;		// Actualizo, está ya ha sido mirada
		switch (tablero[pos.first][pos.second + 1]){
		case 2:
			tablero[pos.first][pos.second + 1] = 3;
			ejecutarExplosion (miBomba.explosionAmarilla(std::pair<int, int>(pos.first, pos.second + 1), DIM));
			chained(std::pair<int, int>(pos.first, pos.second + 1), miBomba);
			break;
		case 3:
			tablero[pos.first][pos.second + 1] = 4;
			ejecutarExplosion (miBomba.explosionNaranja(std::pair<int, int>(pos.first, pos.second + 1), DIM));
			chained(std::pair<int, int>(pos.first, pos.second + 1), miBomba);
			break;
		case 4:
			tablero[pos.first][pos.second + 1] = 0;
			ejecutarExplosion (miBomba.explosionRoja(std::pair<int, int>(pos.first, pos.second + 1), DIM));
			chained(std::pair<int, int>(pos.first, pos.second + 1), miBomba);
			break;
		default:
			break;
		}
	}
}

void Game::endGame() {
	std::cout << "Fin del juego" << std::endl;
	// Mostrar mensaje de fin
	std::string a;
	std::cin >> a;
	reset();
}