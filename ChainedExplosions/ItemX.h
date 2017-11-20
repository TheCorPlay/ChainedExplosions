#ifndef ITEMX_H_INCLUDED
#define ITEMX_H_INCLUDED

#include <utility>
#include <vector>

#include <iostream>
#include <string>


class ItemX {
private:
	int lvl;

	bool casillaValida(const std::pair<int, int> &pos, const int &DIM) const {
		return pos.first >= 0 && pos.first < DIM && pos.second >= 0 && pos.second < DIM;
	}
public:
	int id;

	ItemX() {
		id = 1;
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

	std::vector<std::pair<int, int> > explosionAmarilla(const std::pair<int, int> &pos, const int &DIM) {
		std::vector <std::pair<int, int> > vector;	// Aquí pasaré todas las casillas que explotará
		for (int i = pos.first - lvl; i <= pos.first + lvl; i++) {
			if (casillaValida(std::pair<int, int>(i, pos.second - lvl), DIM))
				vector.push_back(std::pair<int, int>(i, pos.second - lvl));
			if (casillaValida(std::pair<int, int>(i, pos.second + lvl), DIM))
				vector.push_back(std::pair<int, int>(i, pos.second + lvl));
			if (casillaValida(std::pair<int, int>(DIM - i, pos.second - lvl), DIM))
				vector.push_back(std::pair<int, int>(DIM - i, pos.second - lvl));
			if (casillaValida(std::pair<int, int>(DIM - i, pos.second - lvl), DIM))
				vector.push_back(std::pair<int, int>(DIM - i, pos.second + lvl));
		}


		return vector;	// Aún no sabemos si esa casilla podrá explotar, está ocupada por bomba o lo que sea
	}

	std::vector<std::pair<int, int> > explosionNaranja(const std::pair<int, int> &pos, const int &DIM) {
		std::vector <std::pair<int, int> > vector;	// Aquí pasaré todas las casillas que explotará

		if (lvl < 3) {
			for (int i = pos.first - 1; i <= pos.first + 1; i++) {
				for (int j = pos.second - 1; j <= pos.second + 1; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es válida
						!(i == pos.first && j == pos.second))				// y no es la mía
						vector.push_back(std::pair<int, int>(i, j));
				}
			}
		}
		else {	// Básicamente la explosión roja
			for (int i = pos.first - 2; i <= pos.first + 2; i++) {
				for (int j = pos.second - 2; j <= pos.second + 2; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es válida
						!(i == pos.first && j == pos.second))				// y no es la mía
						vector.push_back(std::pair<int, int>(i, j));
				}
			}
		}

		return vector;	// Aún no sabemos si esa casilla podrá explotar, está ocupada por bomba o lo que sea
	}

	std::vector<std::pair<int, int> > explosionRoja(const std::pair<int, int> &pos, const int &DIM) {
		std::vector <std::pair<int, int> > vector;	// Aquí pasaré todas las casillas que explotará

		if (lvl < 3) {
			for (int i = pos.first - 2; i <= pos.first + 2; i++) {
				for (int j = pos.second - 2; j <= pos.second + 2; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es válida
						!(i == pos.first && j == pos.second))				// y no es la mía
						vector.push_back(std::pair<int, int>(i, j));
				}
			}
		}
		else {
			for (int i = pos.first - 3; i <= pos.first + 3; i++) {
				for (int j = pos.second - 3; j <= pos.second + 3; j++) {
					if (casillaValida(std::pair<int, int>(i, j), DIM) &&	// Si la casilla es válida
						!(i == pos.first && j == pos.second)) {				// y no es la mía
						vector.push_back(std::pair<int, int>(i, j));
					}
				}
			}
		}

		return vector;	// Aún no sabemos si esa casilla podrá explotar, está ocupada por bomba o lo que sea
	}
};

#endif // ITEMX_H_INCLUDED