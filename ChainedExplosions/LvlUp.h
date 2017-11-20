#ifndef LVLUP_H_INCLUDED
#define LVLUP_H_INCLUDED

#include <cstdlib>

class LvlUp {
private:
	int grafico;
	int estadoGrafico;

public:
	LvlUp() {
		grafico = rand() % 4;
		estadoGrafico = 0;
		timeLvl = 0;
	}

	int timeLvl;

	int getGrafico() {
		return grafico;
	}

	int getEstadoGrafico() {
		return estadoGrafico;
	}

	void nextEstadoGrafico() {
		estadoGrafico = (1 + estadoGrafico) % 4;
	}
};

#endif // LVLUP_H_INCLUDED