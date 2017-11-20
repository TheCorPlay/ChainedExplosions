#include "Upgrade.h"

#include <fstream>

#include <vector>

std::vector<std::string> u_split(std::string input, std::string separators) {
	std::vector<std::string> r;
	unsigned int wordStart = 0;
	bool inWord = true;
	bool isSeparator = false;

	for (unsigned int pos = 0; pos<input.size(); pos++) {
		isSeparator = separators.find(input[pos]) != std::string::npos;
		if (isSeparator && inWord) {
			inWord = false;
			r.push_back(input.substr(wordStart, pos - wordStart));
		}
		else if (!isSeparator && !inWord) {
			inWord = true;
			wordStart = pos;
		}
	}

	if (inWord)
		r.push_back(input.substr(wordStart));

	return r;
}

int u_cstoi(const std::string &text)
{
	unsigned int i = 0;
	bool positive = true;
	int sol = 0;

	while (positive && i < text.length() && (text[i] > 57 || text[i] < 48 || text[i] == '-')) {
		if (text[i] == '-')
			positive = false;
		i++;
	}

	while (i++ <= text.length())
		if (text[i - 1] <= 57 && text[i - 1] >= 48)
			sol = sol * 10 + text[i - 1] - 48;

	if (!positive)
		sol = -sol;

	return sol;
}

Upgrade::Upgrade() {
	for (int i = 0; i < N_UPGRADES; i++) {
		lvlUpgrades[i] = 0;
	}
}

Upgrade::Upgrade(std::string dir) {
	Upgrade();
	cargar(dir);
}

bool Upgrade::cargar(std::string dir) {
	std::ifstream fichero;
	std::string datos;

	fichero.open(dir);

	if (fichero.is_open()) {
		getline(fichero, datos);
		fichero.close();
	}
	else {
		return false;
	}

	encrypt(datos);

	std::vector<std::string> vectorDatos = u_split(datos, " ");
	for (std::vector<std::string>::iterator it = vectorDatos.begin(); it != vectorDatos.end(); ++it) {
		lvlUpgrades.push_back(u_cstoi(*it));
	}

	return true;
}

void Upgrade::guardar(std::string dir) {
	std::ofstream fichero;

	fichero.open(dir);
	if (fichero.is_open()) {
		std::string datos = "";
		for (std::vector<int>::iterator it = lvlUpgrades.begin(); it != lvlUpgrades.end(); ++it) {
			datos += std::to_string(*it) + " ";
		}

		encrypt(datos);
		fichero << datos; // meto los datos
		fichero.close();
	}
}

void Upgrade::encrypt(std::string &in) {
	for (unsigned int temp = 0; temp < in.size(); temp++) {
		in[temp] ^= 'f';
	}
}