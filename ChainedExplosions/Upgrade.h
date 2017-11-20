#ifndef UPGRADE_H_INCLUDED
#define UPGRADE_H_INCLUDED

#include <string>
#include <vector>

class Upgrade {
private:
	const static int N_UPGRADES = 10;

	std::vector<int> lvlUpgrades;
	
public:
	Upgrade();
	Upgrade(std::string dir);
	bool cargar(std::string dir);
	void guardar(std::string dir);

private:
	void encrypt(std::string &in);
};

#endif // UPGRADE_H_INCLUDED