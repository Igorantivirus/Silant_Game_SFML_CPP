#pragma once

#include<fstream>
#include<vector>

#include"Helper.hpp"
#include"ResourceMeneger.hpp"

#define KEYBOARD_SETTINGS_FILE_NAME "Settings/keyboardSettings.txt"

#define GAME_INFO_FILE "InfoFiles/InfoMenu.txt"

#define GAMEDATA_FILE "gamedata.txt"

#define OBJECTTEXTURS_INFO "InfoFiles\\objectsinfo.txt"

namespace Package
{
	struct ObjectP
	{
		unsigned int ID;
		sf::IntRect spriteRect;
		sf::FloatRect barierBox;
	};
}

class ReadWrite
{
public:

	#pragma region ReadWrite

	template<typename type>
	static bool getline(std::basic_ifstream<type, std::char_traits<type>>& read, sf::String& str)
	{
		str.clear();
		if (!read.is_open() || read.eof())
			return false;
		sf::Uint32 pr = read.get();
		while (pr != '\n' && !read.eof())
		{
			str += pr;
			pr = read.get();
		}
		return true;
	}

	template<typename type>
	static bool getlineToStopSymbol(std::basic_ifstream<type, std::char_traits<type>>& read, sf::String& str, const char stop)
	{
		str.clear();
		if (!read.is_open() || read.eof())
			return false;
		sf::Uint32 pr = read.get();
		while (pr != '\n' && !read.eof() && pr != stop)
		{
			str += pr;
			pr = read.get();
		}
		return true;
	}

	template<typename type>
	static bool getlineUTF8(std::basic_ifstream<type, std::char_traits<type>>& read, sf::String& str)
	{
		str.clear();
		if (!read.is_open() || read.eof())
			return false;
		int pr = read.get();
		while (pr != '\n' && !read.eof())
		{
			if (pr == 208)
				str += sf::Uint32(read.get() + 896);
			else if (pr == 209)
				str += sf::Uint32(read.get() + 960);
			else
				str += sf::Uint32(pr);
			pr = read.get();
		}
		return true;
	}

	template<typename type>
	static void write(std::basic_ofstream<type, std::char_traits<type>>& write, const sf::String& str)
	{
		if (write.eof() || !write.is_open())
			return;
		for (const auto& i : str)
			write << static_cast<type>(i);
	}

	#pragma endregion

	#pragma region KeyBoard

	static std::vector<sf::String> ReadKeyBoardNames()
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

	static std::vector<sf::Keyboard::Key> ReadKeyBoard()
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
	static void WriteKeyBoard(std::vector<sf::Keyboard::Key> keys)
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

	#pragma endregion

	static sf::String ReadInfoGame()
	{
		std::ifstream read(GAME_INFO_FILE);
		sf::String res, pr;
		while (getlineUTF8(read, pr))
			res += pr + '\n';
		return res;
	}
	static int GameDataReader(unsigned lineNumber)
	{
		std::ifstream read(GAMEDATA_FILE);
		std::string pr;

		for (unsigned i = 0; i < lineNumber; ++i)
			std::getline(read, pr);

		int val;
		read >> val;

		read.close();
		return val;
	}

	static Package::ObjectP ReadObjectInfo(const unsigned int ID)
	{
		Package::ObjectP res;

		std::ifstream read(OBJECTTEXTURS_INFO);
		std::string pr;
		for (unsigned i = 0u; i < ID; ++i)
			std::getline(read, pr);

		read >> 
			res.ID >>
			res.spriteRect.left >> res.spriteRect.top >> res.spriteRect.width >> res.spriteRect.height >>
			res.barierBox.width >> res.barierBox.height >>res.barierBox.left >> res.barierBox.top;

		read.close();
		return res;
	}
};