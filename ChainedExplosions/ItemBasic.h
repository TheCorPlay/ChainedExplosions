#ifndef ITEMBASIC_H_INCLUDED
#define ITEMBASIC_H_INCLUDED

#include <utility>
#include <vector>

#include <iostream>
#include <string>

#include "Item.h"

// Clase derivada de Item
class ItemBasic{
private:
	int lvl;

	bool casillaValida(const std::pair<int, int> &pos, const int &DIM) const {
		return pos.first >= 0 && pos.first < DIM && pos.second >= 0 && pos.second < DIM;
	}
public:
	int id;

	ItemBasic() {
		id = 0;					// Id que sirve para saber que item es
		lvl = 1;
	}

	void resetLvl() {
		lvl = 1;
	}

	void aumentaLvl() {
		lvl++;
	}

	void bajaLvl() {
		if (lvl > 1)
			lvl--;
	}

	// Definici�n b�sica de la explosi�n
	std::vector<std::pair<int, int> > explosionAmarilla(const std::pair<int, int> &pos, const int &DIM) {
		std::vector <std::pair<int, int> > vector;	// Aqu� pasar� todas las casillas que explotar�

		if (lvl < 3) {
			for (int i = pos.first - lvl; i <= pos.first + lvl; i++) {
				if (i < 0)
					i = 0;
				for (int j = pos.second - lvl; j <= pos.second + lvl; j++) {
					if (j < 0)
						j = 0;

					if (j > DIM)
						break;

					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es v�lida 
						!(i == pos.first && j == pos.second) &&				// y no es la m�a
						(i == pos.first || j == pos.second)) {				// pero est� en mi fila o columna
						vector.push_back(std::pair<int, int>(i, j));
					}
				}
				if (i > DIM)
					break;
			}
		}
		else { // Equivale a una explosi�n naranja con puntas verdes
			for (int i = pos.first - 1; i <= pos.first + 1; i++) {
				for (int j = pos.second - 1; j <= pos.second + 1; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es v�lida
						!(i == pos.first && j == pos.second))				// y no es la m�a
						vector.push_back(std::pair<int, int>(i, j));
				}
			}

			if (casillaValida(std::pair<int, int>(pos.first - 2, pos.second), DIM))
				vector.push_back(std::pair<int, int>(pos.first - 2, pos.second));
			if (casillaValida(std::pair<int, int>(pos.first, pos.second + 2), DIM))
				vector.push_back(std::pair<int, int>(pos.first, pos.second + 2));
			if (casillaValida(std::pair<int, int>(pos.first + 2, pos.second), DIM))
				vector.push_back(std::pair<int, int>(pos.first + 2, pos.second));
			if (casillaValida(std::pair<int, int>(pos.first, pos.second - 2), DIM))
				vector.push_back(std::pair<int, int>(pos.first, pos.second - 2));
		}


		return vector;	// A�n no sabemos si esa casilla podr� explotar, est� ocupada por bomba o lo que sea
	}

	std::vector<std::pair<int, int> > explosionNaranja(const std::pair<int, int> &pos, const int &DIM) {
		std::vector <std::pair<int, int> > vector;	// Aqu� pasar� todas las casillas que explotar�

		if (lvl < 3) {
			for (int i = pos.first - 1; i <= pos.first + 1; i++) {
				for (int j = pos.second - 1; j <= pos.second + 1; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es v�lida
						!(i == pos.first && j == pos.second))				// y no es la m�a
						vector.push_back(std::pair<int, int>(i, j));
				}
			}
		}
		else {	// B�sicamente la explosi�n roja
			for (int i = pos.first - 2; i <= pos.first + 2; i++) {
				for (int j = pos.second - 2; j <= pos.second + 2; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es v�lida
						!(i == pos.first && j == pos.second))				// y no es la m�a
						vector.push_back(std::pair<int, int>(i, j));
				}
			}
		}

		return vector;	// A�n no sabemos si esa casilla podr� explotar, est� ocupada por bomba o lo que sea
	}

	std::vector<std::pair<int, int> > explosionRoja(const std::pair<int, int> &pos, const int &DIM) {
		std::vector <std::pair<int, int> > vector;	// Aqu� pasar� todas las casillas que explotar�

		if (lvl < 3) {
			for (int i = pos.first - 2; i <= pos.first + 2; i++) {
				for (int j = pos.second - 2; j <= pos.second + 2; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es v�lida
						!(i == pos.first && j == pos.second))				// y no es la m�a
						vector.push_back(std::pair<int, int>(i, j));
				}
			}
		}
		else {
			for (int i = pos.first - 3; i <= pos.first + 3; i++) {
				for (int j = pos.second - 3; j <= pos.second + 3; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es v�lida
						!(i == pos.first && j == pos.second)) {				// y no es la m�a
						vector.push_back(std::pair<int, int>(i, j));
					}
				}
			}
		}

		return vector;	// A�n no sabemos si esa casilla podr� explotar, est� ocupada por bomba o lo que sea
	}
};

#endif // ITEMBASIC_H_INCLUDED