#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include <utility>
#include <vector>

#include <iostream>
#include <string>

#include "ItemBasic.h"
#include "ItemX.h"

class Item {
protected:
	int lvl;

	virtual bool casillaValida(const std::pair<int, int> &pos, const int &DIM) const = 0;

public:
	int id;
	Item() {
		lvl = 1;
	}

	virtual void resetLvl() = 0;

	virtual void aumentaLvl() = 0;

	virtual void bajaLvl() = 0;

	virtual std::vector<std::pair<int, int> > explosionAmarilla(const std::pair<int, int> &pos, const int &DIM) = 0;
	virtual std::vector<std::pair<int, int> > explosionNaranja(const std::pair<int, int> &pos, const int &DIM) = 0;
	virtual std::vector<std::pair<int, int> > explosionRoja(const std::pair<int, int> &pos, const int &DIM) = 0;
};

#endif // ITEM_H_INCLUDED