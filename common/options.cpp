//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "options.h"
#include "utils.h"
#include <fstream>

using namespace std;

bool Options::sound = true;
bool Options::fullscreen = false;
bool Options::soundConfig = true;
bool Options::fullscreenConfig = false;

void Options::setSound(bool s) {
	soundConfig=s;
	setSoundSession(s);
}
void Options::setSoundSession(bool s) {
	cout << "sound : " << s << endl;
	sound = s;
}
void Options::setFullscreen(bool f) {
	fullscreen=f;
	fullscreenConfig=f;
}
bool Options::fullscreenOn() {
	return fullscreen;
}
bool Options::soundOn() {
	return sound;
}
void Options::handleArguments(int argc, char* argv[]) {
	string param;
	for (int k=1; k<argc; k++) {
		param=string(argv[k]);
		if (param=="--no-sound") sound=false;
		else if (param=="--windowed") fullscreen=false;
	}
}

void Options::save() {
	string filename = config_file();
	ofstream file(filename.c_str());
	if (file) {
		file << soundConfig;
		file << fullscreenConfig;
	} else {
		cout << "impossible de sauvegarder le fichier de config (chemin : " << filename << ")" << endl;
	}
}

void Options::load() {
	string filename = string(config_file());
	ifstream file(filename.c_str());
	if(file) {
		file >> soundConfig;
		file >> fullscreenConfig;
		cout << "d'après le fichier de config, sound =" << sound << " et fullscreen =" << fullscreen << endl;
	} else {
		cout << "fichier de config \"" << config_file() << "\" inexistant, configuration par défaut appliquée" << endl;
		soundConfig = true;
		fullscreenConfig = false;
	}
	sound = soundConfig;
	fullscreen = fullscreenConfig;
}
