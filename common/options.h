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
#ifndef __OPTIONS_H
#define __OPTIONS_H

class Options {
	public:
		static void save();
		static void load();
		static void setSound(bool s);
		static void setSoundSession(bool s);
		static void setFullscreen(bool f);
		static void handleArguments(int argc, char* argv[]);
		static bool fullscreenOn();
		static bool soundOn();
	private:
		static bool soundConfig;		//what is in the config file
		static bool fullscreenConfig;	//what is in the config file
		static bool fullscreen;			//the value for this instance (can be modified by command line arguments)
		static bool sound;				//the value for this instance (can be modified by command line arguments)
};

#endif
