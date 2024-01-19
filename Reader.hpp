#pragma once

#include<fstream>
#include<vector>

#include"KeyBoard.hpp"
#include"Helper.hpp"

#define KEYBOARD_SETTINGS_FILE_NAME "Settings/keyboardSettings.txt"

#define GAME_INFO_FILE "Sprites/InfoMenu.txt"

namespace ReadWrite
{
	void ReadKeyBoard(KeyBoard& kb)
	{
		std::ifstream read(KEYBOARD_SETTINGS_FILE_NAME);
		
		std::string pr;
		sf::Keyboard::Key key;
		
		read >> key; kb.SetUp(key);
		read >> key; kb.SetDown(key);
		read >> key; kb.SetLeft(key);
		read >> key; kb.SetRight(key);
		read >> key; kb.SetNext(key);
		read >> key; kb.SetBack(key);
		read >> key; kb.SetInventory(key);
		read >> key; kb.SetFullScreen(key);
		read.close();
	}

	std::vector<sf::String> ReadKeyBoardNames()
	{
		std::ifstream read(KEYBOARD_SETTINGS_FILE_NAME);

		std::vector<sf::String> res;
		sf::String str;
		int pr;
		while (!read.eof())
		{
			read >> pr;
			getlineUTF8(read,str);
			res.push_back(str);
		}
		return res;
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