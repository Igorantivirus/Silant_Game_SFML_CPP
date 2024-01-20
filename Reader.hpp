#pragma once

#include<fstream>
#include<vector>

#include"Helper.hpp"

#define KEYBOARD_SETTINGS_FILE_NAME "Settings/keyboardSettings.txt"

#define GAME_INFO_FILE "Sprites/InfoMenu.txt"

namespace ReadWrite
{
	std::vector<sf::String> ReadKeyBoardNames()
	{
		std::ifstream read(KEYBOARD_SETTINGS_FILE_NAME);

		std::vector<sf::String> res;
		sf::String str;
		int pr;
		while (!read.eof())
		{
			read >> pr;
			getlineUTF8(read, str);
			if (str.getSize() < 15)
				str += std::string(15 - str.getSize(), ' ');
			res.push_back(str);
		}
		return res;
	}

	std::vector<sf::Keyboard::Key> ReadKeyBoard()
	{
		std::ifstream read(KEYBOARD_SETTINGS_FILE_NAME);
		std::vector<sf::Keyboard::Key> res;
		std::string pr;
		int key;
		for (int i = 0; i < 9; ++i)
		{
			read >> key; res.push_back(static_cast<sf::Keyboard::Key>(key)); std::getline(read, pr);

		}
		read.close();
		return res;
	}
	void WriteKeyBoard(std::vector<sf::Keyboard::Key> keys)
	{
		std::ifstream inpr(KEYBOARD_SETTINGS_FILE_NAME);
		std::vector<std::string> pr;
		std::string prstr;
		int pri;
		while (!inpr.eof())
		{
			inpr >> pri;
			std::getline(inpr, prstr);
			pr.push_back(prstr);
		}
		inpr.close();

		std::ofstream w(KEYBOARD_SETTINGS_FILE_NAME);

		w << toInt(keys[0]) << pr[0];
		for (size_t i = 1; i < pr.size() && i < keys.size(); ++i)
			w << '\n' << toInt(keys[i]) << pr[i];
		w.close();
	}


	sf::String ReadInfoGame()
	{
		std::ifstream read(GAME_INFO_FILE);
		sf::String res, pr;
		while (getlineUTF8(read, pr))
			res += pr + '\n';
		return res;
	}
}